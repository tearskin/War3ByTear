#ifndef _MEMORY_UTIL_
#define _MEMORY_UTIL_

#include<Windows.h>
//��ģ���ַ
#include<TlHelp32.h>
#include<tchar.h>
#include<iostream>

//�ڴ��д����  ���ֳɺ���    ֱ�����ö�     WriteBytes�����һ�ѷ�����ʵ����WriteBytes
//�����Ͱ���90 90 NOP  ����  76   �ȵ�  jmp��    huibian���������
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