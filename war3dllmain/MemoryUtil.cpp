#include "MemoryUtil.h"

////请加入粉丝群  QQ725875210   / 435539500      
//为什么每个写函数都要用SetmemWriteable  即VirtualProtect 怕有些地方有页面保护属性无法写
//一般而言非内核再怎么样  即便是内核 你都可以读  写就不行  读肯定型  不过内核肯定要内核Driver才能读和写
void SetMemWritable(DWORD address, SIZE_T size)
{
	DWORD old;  
	VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
	//virtualprotect  页面保护属性  address参数是地址  size是保护大小   下面一个是设置保护属性
	//最后面一个则是差不多   数据类型大小
}

DWORD ReadDWORD(DWORD address)
{
	PWORD p = (PWORD)address;
	  //貌似是指针    地址   然后*取该地址值  &貌似是取其地址   对 就这样
	return *p;
}

void WriteDWORD(DWORD address, DWORD value)
{
	SetMemWritable(address, 4);  //多为1024    看CE吧 一个汇编码大小  是多少 字节
	PWORD p = (PWORD)address;
	*p = value;//上面是读  这里是写  不过就是指针赋值
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
	*p = value;   //说下
}

void WriteBytes(DWORD address, SIZE_T n, DWORD value, ...)
{
	for (int i = 0; i < n; i++) {
		WriteByte(address + i, (&value)[i]);  //这个 有点budong不懂 
	}
}

//jiexialai接下来的写入方法函数基本不用 不打了s
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



//头痛 还是要游戏pid  人裂开 
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

