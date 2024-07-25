#include "MemoryUtil.h"

////������˿Ⱥ  QQ725875210   / 435539500      
//Ϊʲôÿ��д������Ҫ��SetmemWriteable  ��VirtualProtect ����Щ�ط���ҳ�汣�������޷�д
//һ����Է��ں�����ô��  �������ں� �㶼���Զ�  д�Ͳ���  ���϶���  �����ں˿϶�Ҫ�ں�Driver���ܶ���д
void SetMemWritable(DWORD address, SIZE_T size)
{
	DWORD old;  
	VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
	//virtualprotect  ҳ�汣������  address�����ǵ�ַ  size�Ǳ�����С   ����һ�������ñ�������
	//�����һ�����ǲ��   �������ʹ�С
}

DWORD ReadDWORD(DWORD address)
{
	PWORD p = (PWORD)address;
	  //ò����ָ��    ��ַ   Ȼ��*ȡ�õ�ֵַ  &ò����ȡ���ַ   �� ������
	return *p;
}

void WriteDWORD(DWORD address, DWORD value)
{
	SetMemWritable(address, 4);  //��Ϊ1024    ��CE�� һ��������С  �Ƕ��� �ֽ�
	PWORD p = (PWORD)address;
	*p = value;//�����Ƕ�  ������д  ��������ָ�븳ֵ
}

BYTE ReadByte(DWORD address)
{
	PBYTE p = (PBYTE)address;

	return *p;
}

void WriteByte(DWORD address, BYTE value)
{
	SetMemWritable(address, 4);
	PBYTE p = (PBYTE)address;
	*p = value;   //˵��
}

void WriteBytes(DWORD address, SIZE_T n, DWORD value, ...)
{
	for (int i = 0; i < n; i++) {
		WriteByte(address + i, (&value)[i]);  //��� �е�budong���� 
	}
}

//jiexialai��������д�뷽�������������� ������s
void WriteString(DWORD address, LPCSTR str)
{
	for (int i = 0; str[i]; i++) {
		WriteByte(address + i, str[i]);
	}
}

void WriteCall(DWORD address, DWORD func)
{
	DWORD rel32 = func - (address + 5);
	WriteByte(address, 0xE8);
	WriteDWORD(address + 1, rel32);
}

void WriteNop(DWORD address, SIZE_T size)
{
	for (int i = 0; i < size; i++) {
		WriteByte(address + i, 0x90);
	}
}

void WriteJmpRe132(DWORD address, DWORD dest)
{
	DWORD rel32 = dest - (address + 5);
	WriteByte(address, 0xE9);
	WriteDWORD(address + 1, rel32);
}



//ͷʹ ����Ҫ��Ϸpid  ���ѿ� 
DWORD GetModuleBaseAddress( const TCHAR* szModuleName) {
	DWORD dwProcessId = GetCurrentProcessId();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	MODULEENTRY32 ModuleEntry32 = { sizeof(MODULEENTRY32) };
	BOOL bModuleFound = Module32First(hSnapshot, &ModuleEntry32);

	DWORD dwModuleBaseAddress = 0;

	while (bModuleFound) {
		if (!_tcsicmp(ModuleEntry32.szModule, szModuleName)) {
			dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
			break;
		}

		bModuleFound = Module32Next(hSnapshot, &ModuleEntry32);
	}

	CloseHandle(hSnapshot);

	return dwModuleBaseAddress;
}

