#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include<filesystem>
#include<string>
#include <direct.h>
using namespace std;

DWORD GetPidByName(LPCWSTR lpName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnap)
	{
		cout << "�������̿���ʧ��" << endl;
		return 0;
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);
	do
	{
		if (!_wcsicmp(lpName, pe.szExeFile))
		{
			return pe.th32ProcessID;
		}
	} while (Process32Next(hSnap, &pe));
	return 0;
}

// ��Ȩ
int EnableDebugPrivilege()
{
	HANDLE token;
	TOKEN_PRIVILEGES tp;
	// �򿪽������ƻ�
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		cout << "�򿪽�������ʧ��" << endl;
		return 0;
	}
	//  ��ȡ���̱���ΨһID
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		cout << "��ȡLUIDʧ��" << endl;
		return 0;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	// ��������Ȩ��
	if (!AdjustTokenPrivileges(token, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		cout << "��Ȩʧ��" << endl;
		return 0;
	}
	return 1;
}

int main()
{
	SetConsoleTitle(L"�˴��ڿɹر�");
	std::cout << "��  ��  ע   ��  ��  Ϸ  ȷ��dll�ļ������exe�ļ�ͬһ·�� ���� ����  ���ģ�����" << endl;
	if (!EnableDebugPrivilege())
	{
		cout << "��Ȩʧ��" << endl;
		return 0;
	}
	DWORD dwTargetPid = GetPidByName(L"War3.exe");
	if (!dwTargetPid)
	{
		cout << "��ȡPIDʧ��" << endl;
		return 0;
	}
	cout << "PID: " << dwTargetPid << endl;

	// �򿪽���
	HANDLE hTarget = OpenProcess(PROCESS_ALL_ACCESS, false, dwTargetPid);
	if (!hTarget)
	{
		cout << "�򿪽���ʧ��" << GetLastError() << endl;
		return 0;
	}

	void* pLoadLibFuncParam = nullptr;
	pLoadLibFuncParam = VirtualAllocEx(hTarget, 0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pLoadLibFuncParam == nullptr)
	{
		cout << "�����ڴ�ʧ��" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	//dll����λ��
	
	char buffer[256];
	if (_getcwd(buffer, sizeof(buffer)) != nullptr) {
		std::cout << "DLL����·��(ע�����ر�): " << buffer << std::endl;
	}
	LPCTSTR lpParam =L"\\d3dhook_imgui.dll";
	strcat(buffer, (char*)lpParam);
	std::cout << "���·��:" << buffer << endl;
	
	if (!WriteProcessMemory(hTarget, pLoadLibFuncParam, (LPCVOID)lpParam, (wcslen(lpParam) + 1) * sizeof(TCHAR), NULL))
	{
		cout << "д���ڴ�ʧ��" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	HMODULE hNtdll = LoadLibrary(L"kernel32.dll");
	if (!hNtdll)
	{
		cout << "����ģ�����" << GetLastError() << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "ģ����: " << hNtdll << endl;
	void* pLoadLibrary = nullptr;
	pLoadLibrary = GetProcAddress(hNtdll, "LoadLibraryW");
	if (pLoadLibrary == nullptr)
	{
		cout << "�Ҳ�������" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "������ַ: " << pLoadLibrary << endl;
	DWORD dwThreadId = 0;
	HANDLE hRemoteThread = CreateRemoteThread(hTarget, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)pLoadLibFuncParam, 0, &dwThreadId);
	if (!hRemoteThread)
	{
		cout << "��������ʧ��" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "���н���" << hRemoteThread << endl;
	getchar();
	getchar();
	CloseHandle(hTarget);
	system("pause");
	return 0;
}
