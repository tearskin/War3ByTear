#ifndef _MEMORY_UTIL_
#define _MEMORY_UTIL_

#include<Windows.h>
//读模块基址
#include<TlHelp32.h>
#include<tchar.h>
#include<iostream>

//内存读写工具  线现成函数    直接引用段     WriteBytes后面的一堆方法其实都是WriteBytes
//不过就帮你90 90 NOP  或者  76   等等  jmp了    huibian会变吗汇编码
void SetMemWritable(DWORD address, SIZE_T size);

DWORD ReadDWORD(DWORD address);

void  WriteDWORD(DWORD address, DWORD value);

BYTE ReadByte(DWORD  address);

void WriteByte(DWORD address, BYTE value);

void WriteBytes(DWORD address, SIZE_T n, DWORD value, ...);

void WriteString(DWORD address, LPCSTR str);

void WriteCall(DWORD address, DWORD func);

void WriteNop(DWORD address, SIZE_T size);

void WriteJmpRe132(DWORD address, DWORD  dest);     

DWORD GetModuleBaseAddress(const TCHAR* szModuleName);

#endif