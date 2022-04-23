
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

	//InitChanelShnModbus(&g_Comm.mbEncoder);


	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbEncoder);
	g_Comm.mbEncoder.Frame.TimerPost.Timeout=10;
	g_Comm.mbEncoder.Frame.TimerConn.Timeout=1000;
	g_Comm.mbEncoder.Frame.TimerAck.Timeout=80;

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

	if (PauseModbus == 0)
	{
		ModBusUpdate(&g_Comm.mbEncoder);  

		if (p->mbEncoder.Stat.Status.bit.Busy == 0)
		{
			CommandATS48++;
			p->mbEncoder.Stat.Status.bit.Ready = 0;

			switch (CommandATS48)
			{
			case 1:
                                        //mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.State1), 1);
					break;
			case 2:
					//mb_write_ATS48(&g_Comm, GetAdr(ramGroupATS.Control1), 1, g_Ram.ramGroupATS.Control1.all);
	
                                  break;
			case 3:
				CommandATS48 = 0;
				break;
			}
		}
		Comm_ControlModbusUpdateAltistar48(&g_Comm);

	}
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

