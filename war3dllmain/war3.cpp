#include "war3.h"
#include "logger.h"
//请加入粉丝群  QQ725875210   / 435539500      

HANDLE WarHwnd = NULL;
DWORD pid = 0;

HANDLE getProcess()
{
    pid = GetCurrentProcessId();
    WarHwnd = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    return  WarHwnd;
}


void WriteMemory(void* value, DWORD valueSize, ...)
{
    
    HANDLE g_processHandle = getProcess();   //需要主动写一个获取HANDLE的方法或者得到一个HANDLE
    if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

    DWORD tempValue = 0;

    va_list addresses;
    va_start(addresses, valueSize);
    DWORD offset = 0;
    DWORD lastAddress = 0;
    while ((offset = va_arg(addresses, DWORD)) != -1)
    {
        lastAddress = tempValue + offset;
        ::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
    }
    va_end(addresses);

    ::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

void WriteMemory(void* value, DWORD valueSize, DWORD address) {
    WriteMemory(value, valueSize, address, -1);
}


//  DWORD_PTR lpAddr = (DWORD_PTR)GetProcAddress(GetModuleHandle("user32.dll"), "MessageBoxA");
//LOG_INFO("user32.dll:MessageBoxA addr is -> %p", lpAddr)
DWORD WarAddr = GetModuleBaseAddress(_T("game.dll"));
void open() {
    WarHwnd = getProcess();
   
    LOG_INFO("Game War module address is: %x", WarAddr);
    DWORD cheat = WarAddr + 0x39916;
   // BYTE  s[] = { 0x90,0x90,0x90 };
    WriteBytes(cheat, 3, 144, 144, 144);    //应该是十六进制转十进制才对啊
    LOG_INFO("Game War cheat address is: %x", cheat);
    LOG_INFO("Change Game success!");
  

}

void close() {
    WarHwnd = getProcess();
    //DWORD WarAddr = GetModuleBaseAddress(_T("game.dll"));
    LOG_INFO("Game War module address is: %x", WarAddr);
    DWORD cheat = WarAddr + 0x39916;
   // BYTE  s[] = { 0x89,0x48,0x78 };
    //WriteMemory(s, sizeof(s), cheat);
    WriteBytes(cheat, 3, 137, 72, 120);    //89 48 78
    LOG_INFO("resume game cheat success! 恢复成功");

}

void UnlimitedThings()
{
    WarHwnd = getProcess();
    LOG_INFO("\n启动无限物品成功,如果不懂功能请直接看调试窗!");
    //Game.dll+1F5B99 
    DWORD cheatUnlimitedArtcle = WarAddr + 0x1F5B99;
    WriteBytes(cheatUnlimitedArtcle,2,137,168);  //  89 80    a8是无限物品  80是正常

    //LOG_INFO("\n五分钟给后自动恢复正常\n");
    //Sleep(300000);
   // WriteBytes(cheatUnlimitedArtcle, 2, 137, 128);
    //LOG_INFO("无限物品已经恢复正常!");
}

void immediateExp()
{//Game.dll+50AC22   duan断电
    DWORD  Level = WarAddr + 0x50AC22;
    //89 87 8C 00 00 00 改89 BF 8C 00 00 00
    //写入经验获取   89  BF  8C
    WriteBytes(Level, 3, 137, 191, 140);
    LOG_INFO("主公哪,您现在经验值满啦!一击一万级!");
}

void CD() {//Game.dll+3BBEEC 
    DWORD cdAddr = WarAddr + 0x3BBEEC;
    //无限蓝量eax改ecx  89 8F    只需要第二个是8F   89 87 
    WriteBytes(cdAddr, 2, 137, 143);
    LOG_INFO("无CD目前还没有找到基址 只找到了无限蓝");
}

void coin() {
   // DWORD  coinaddr = WarAddr + 0x23DD53;
     DWORD  coinaddr = WarAddr + 0xBE40A8 ;
     DWORD  s1, s2, s3;
    //Game.dll+BE40A8  
    s1= ReadMemory_DWORD(WarHwnd, coinaddr, false);
    s2 = ReadMemory_DWORD(WarHwnd, (s1 + 0x0C), false);  
    LOG_INFO("金币偏移1:%lx", s2);
    s2 = ReadMemory_DWORD(WarHwnd, (s2 + 0x14), false);
  // LOG_INFO("\n金币偏移2:%lx", s2);
    s3 = ReadMemory_DWORD(WarHwnd, (s2 + 0x78), false);
    LOG_INFO("\n最终偏移值:%d:", s3);
    DWORD x = 9999999;
    WriteMemory_DWORD(WarHwnd, s3, x);
   // LOG_INFO("读出金币基址为:%d", c7);
   // //这是金币人口等等不变动的玩意!
   // BYTE a[] = { 0x57 };
   // WriteByte(coinaddr,87);
}



