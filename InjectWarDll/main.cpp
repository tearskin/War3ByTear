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
		cout << "创建进程快照失败" << endl;
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

// 提权
int EnableDebugPrivilege()
{
	HANDLE token;
	TOKEN_PRIVILEGES tp;
	// 打开进程令牌环
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		cout << "打开进程令牌失败" << endl;
		return 0;
	}
	//  获取进程本地唯一ID
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		cout << "获取LUID失败" << endl;
		return 0;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	// 调整进程权限
	if (!AdjustTokenPrivileges(token, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		cout << "提权失败" << endl;
		return 0;
	}
	return 1;
}

int main()
{
	SetConsoleTitle(L"此窗口可关闭");
	std::cout << "自  动  注   入  游  戏  确保dll文件和这个exe文件同一路径 不可 出现  中文！！！" << endl;
	if (!EnableDebugPrivilege())
	{
		cout << "提权失败" << endl;
		return 0;
	}
	DWORD dwTargetPid = GetPidByName(L"War3.exe");
	if (!dwTargetPid)
	{
		cout << "获取PID失败" << endl;
		return 0;
	}
	cout << "PID: " << dwTargetPid << endl;

	// 打开进程
	HANDLE hTarget = OpenProcess(PROCESS_ALL_ACCESS, false, dwTargetPid);
	if (!hTarget)
	{
		cout << "打开进程失败" << GetLastError() << endl;
		return 0;
	}

	void* pLoadLibFuncParam = nullptr;
	pLoadLibFuncParam = VirtualAllocEx(hTarget, 0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pLoadLibFuncParam == nullptr)
	{
		cout << "申请内存失败" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	//dll所在位置
	
	char buffer[256];
	if (_getcwd(buffer, sizeof(buffer)) != nullptr) {
		std::cout << "DLL所在路径(注入后请关闭): " << buffer << std::endl;
	}
	LPCTSTR lpParam =L"\\d3dhook_imgui.dll";
	strcat(buffer, (char*)lpParam);
	std::cout << "输出路径:" << buffer << endl;
	
	if (!WriteProcessMemory(hTarget, pLoadLibFuncParam, (LPCVOID)lpParam, (wcslen(lpParam) + 1) * sizeof(TCHAR), NULL))
	{
		cout << "写入内存失败" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	HMODULE hNtdll = LoadLibrary(L"kernel32.dll");
	if (!hNtdll)
	{
		cout << "加载模块错误" << GetLastError() << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "模块句柄: " << hNtdll << endl;
	void* pLoadLibrary = nullptr;
	pLoadLibrary = GetProcAddress(hNtdll, "LoadLibraryW");
	if (pLoadLibrary == nullptr)
	{
		cout << "找不到函数" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "函数地址: " << pLoadLibrary << endl;
	DWORD dwThreadId = 0;
	HANDLE hRemoteThread = CreateRemoteThread(hTarget, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)pLoadLibFuncParam, 0, &dwThreadId);
	if (!hRemoteThread)
	{
		cout << "创建进程失败" << endl;
		CloseHandle(hTarget);
		return 0;
	}
	cout << "运行结束" << hRemoteThread << endl;
	getchar();
	getchar();
	CloseHandle(hTarget);
	system("pause");
	return 0;
}
