#include "war3.h"
#include "logger.h"
//������˿Ⱥ  QQ725875210   / 435539500      

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
    
    HANDLE g_processHandle = getProcess();   //��Ҫ����дһ����ȡHANDLE�ķ������ߵõ�һ��HANDLE
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
    WriteBytes(cheat, 3, 144, 144, 144);    //Ӧ����ʮ������תʮ���ƲŶ԰�
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
    LOG_INFO("resume game cheat success! �ָ��ɹ�");

}

void UnlimitedThings()
{
    WarHwnd = getProcess();
    LOG_INFO("\n����������Ʒ�ɹ�,�������������ֱ�ӿ����Դ�!");
    //Game.dll+1F5B99 
    DWORD cheatUnlimitedArtcle = WarAddr + 0x1F5B99;
    WriteBytes(cheatUnlimitedArtcle,2,137,168);  //  89 80    a8��������Ʒ  80������

    //LOG_INFO("\n����Ӹ����Զ��ָ�����\n");
    //Sleep(300000);
   // WriteBytes(cheatUnlimitedArtcle, 2, 137, 128);
    //LOG_INFO("������Ʒ�Ѿ��ָ�����!");
}

void immediateExp()
{//Game.dll+50AC22   duan�ϵ�
    DWORD  Level = WarAddr + 0x50AC22;
    //89 87 8C 00 00 00 ��89 BF 8C 00 00 00
    //д�뾭���ȡ   89  BF  8C
    WriteBytes(Level, 3, 137, 191, 140);
    LOG_INFO("������,�����ھ���ֵ����!һ��һ��!");
}

void CD() {//Game.dll+3BBEEC 
    DWORD cdAddr = WarAddr + 0x3BBEEC;
    //��������eax��ecx  89 8F    ֻ��Ҫ�ڶ�����8F   89 87 
    WriteBytes(cdAddr, 2, 137, 143);
    LOG_INFO("��CDĿǰ��û���ҵ���ַ ֻ�ҵ���������");
}

void coin() {
   // DWORD  coinaddr = WarAddr + 0x23DD53;
     DWORD  coinaddr = WarAddr + 0xBE40A8 ;
     DWORD  s1, s2, s3;
    //Game.dll+BE40A8  
    s1= ReadMemory_DWORD(WarHwnd, coinaddr, false);
    s2 = ReadMemory_DWORD(WarHwnd, (s1 + 0x0C), false);  
    LOG_INFO("���ƫ��1:%lx", s2);
    s2 = ReadMemory_DWORD(WarHwnd, (s2 + 0x14), false);
  // LOG_INFO("\n���ƫ��2:%lx", s2);
    s3 = ReadMemory_DWORD(WarHwnd, (s2 + 0x78), false);
    LOG_INFO("\n����ƫ��ֵ:%d:", s3);
    DWORD x = 9999999;
    WriteMemory_DWORD(WarHwnd, s3, x);
   // LOG_INFO("������һ�ַΪ:%d", c7);
   // //���ǽ���˿ڵȵȲ��䶯������!
   // BYTE a[] = { 0x57 };
   // WriteByte(coinaddr,87);
}



