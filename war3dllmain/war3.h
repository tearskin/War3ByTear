#pragma once
#include<Windows.h>
#include "MemoryUtil.h"
#include "memory.h"
extern HANDLE  WarHwnd;
extern  DWORD pid;
//Ä£¿é»ùÖ·
//DWORD GetModuleBaseAddress(DWORD dwProcessId, const TCHAR* szModuleName);

HANDLE  getProcess();

void WriteMemory(void* value, DWORD valueSize, DWORD address);

void WriteMemory(void* value, DWORD valueSize, ...);

void open();
void close();

void UnlimitedThings();

void CD();

void immediateExp();

void coin();