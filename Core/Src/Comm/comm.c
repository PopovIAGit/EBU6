
#include "comm.h"
#include "g_Ram.h"

TComm	g_Comm;
Uint16 ASU_Data[10];
Uns CommandATS48=0;

Uns TmpPosition = 0;
static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count);
//---------------------------------------------------
void Comm_Init(TComm *p)
{

	InitChanelAsuModbus(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbAsu);
}

//---------------------------------------------------
void Comm_Update(TComm *p)
{

        if(g_Ram.Comands.RsReset != 0 )
        {  
                InitChanelAsuModbus(&g_Comm.mbAsu);
                ModBusSlaveReset(&p->mbAsu);
                p->mbAsu.Serial.RsReset = 0;
                g_Ram.Comands.RsReset = 0;
        }

	if (PauseModbus == 0)
	{
		ModBusUpdate(&g_Comm.mbAsu); 			
	}
          
            /*тут добавлять связь с БРП*/ 
}

static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count)
{
	memcpy(Port->Packet.Data, &Data[Addr], Count);
	return 0;
}

void Comm_50HzCalc(TComm *p)
{
	
}

void Comm_CommandUpdate(TComm *p)
{

}
//---------------------------------------------------

