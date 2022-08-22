/*#include "main.h"
#include "g_Core.h"

#include "comm_SerialComm.h"

#include "stat.h"*/

#include "comm.h"
#include "g_Ram.h"

Bool MbBkpConnect = False;
Bool MbAsuConnect = False;
Bool MbShnConnect = False;
Bool MbBtConnect = False;

static Uns BaudRates[7] = SCI_DEFAULT_BAUD_RATES;
static Uns BrrValues[7] = SCI_DEFAULT_BAUD_RATES;//SCI_DEFAULT_BRR_VALUES;

#define IM_MB_START_ADDR		65123
#define CHECK_IM_MB_ADDR(Addr)	        (Addr==IM_MB_START_ADDR)
#define IM_MB_START_DATA_ADDR		65124
#define CHECK_IM_MB_DATA_ADDR(Addr)	(Addr==IM_MB_START_DATA_ADDR)

 Byte UpdatePacket(TMbPacket *Packet);
 Byte WriteData(Uns Addr, Uns *Data, Uns Count);
 void AsuMbSetTr(Byte Lev);

 void AsuMbSetTr(Byte Lev)  
{
 //  HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)Lev);
}

//---------------------------------------------------
void InitChanelAsuModbus(TMbHandle hPort)
{
	//hPort->Params.ChannelID = ASU_SCI_ID;//
	hPort->Params.Mode     	= MB_SLAVE;//
	hPort->Params.Slave    	= g_Ram.UserParam.RsStation;//
	hPort->Params.BaudRate	= BaudRates[g_Ram.UserParam.RsBaudRate];//
	hPort->Params.UartBaud 	= BrrValues[g_Ram.UserParam.RsBaudRate];//
	hPort->Params.Parity   	= g_Ram.UserParam.RsMode;//

/*	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = 2;//MB_SCALE;
//	hPort->Params.ConnTimeout = Serial->RsIndicTime * 100;
	hPort->Params.ConnTimeout = 2;//10
	hPort->Params.RxDelay     = 10;//
//	hPort->Params.TxDelay     = Serial->RsWaitTime - 3;
	hPort->Params.TxDelay     = 0;//10
	hPort->Params.AckTimeout  = 6;//1000
	hPort->Params.TrEnable    = &AsuMbSetTr;
	hPort->Frame.TimerPre.Timeout = 1; //
	*/
	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = MB_SCALE;
	hPort->Params.ConnTimeout = 40;
	hPort->Params.RxDelay     = 10;
	hPort->Params.TxDelay     = 10;
	hPort->Params.AckTimeout  = 1;//1000;
	hPort->Params.TrEnable    = &AsuMbSetTr; // СѓРїСЂР°РІР»РµРЅРёРµ РЅР°РїСЂР°РІР»РµРЅРёРµРј
	hPort->Frame.TimerPre.Timeout = 1;

	hPort->Params.HardWareType	= 0;
	hPort->Packet.ParamMode	= 0;

	hPort->Params.TrEnable(0);

}



//---------------------------------------------------

void SerialCommInit(TMbHandle hPort)
{
	ModBusSlaveReset(hPort);
}
//---------------------------------------------------
void SerialCommTimings(TMbHandle hPort)
{
	ModBusTimings(hPort);
}
//---------------------------------------------------
void ModBusSlaveReset(TMbHandle hPort)
{
	ModBusInit(hPort);
}
//---------------------------------------------------
void ModBusUpdate(TMbHandle hPort)
{
	TMbPacket *Packet = &hPort->Packet;

	if (IsSlave())
	{
		if (Packet->Request)
		{
			Packet->Exception = UpdatePacket(Packet);
			Packet->Response  = Packet->Request;
			Packet->Request   = 0;
		}
	}

	hPort->Serial.RsState = Packet->Exception; //???MbConnect = !Packet->Exception;
	
	ModBusInvoke(hPort);
}
//---------------------------------------------------
void ModBusControl(TMbHandle hPort)
{

}

 Byte UpdatePacket(TMbPacket *Packet)
{
	Uns Addr=0, Res=0, Tmp=0, i=0;

		Addr = Packet->Addr;

		Tmp  = Packet->Addr + Packet->Count - 1;

		if (Tmp <= (RAM_DATA_SIZE + 5)) {Res = 1;}
		else if (CHECK_IM_MB_ADDR(Addr))	{Res = 5;}
		else if (CHECK_IM_MB_DATA_ADDR(Addr))	{Res = 6;}
		else if (Addr==65125)	{Res = 6;}
		else if (Addr==65345)	{Res = 7;}
		else {Res = 0;}

		if (!Res && Packet->Request!=MB_REPORT_ID) {return EX_ILLEGAL_DATA_ADDRESS;}

		switch(Packet->Request)
		{

			case MB_REPORT_ID:
				Packet->Data[0] = (DEVICE_ID>>8)&0x00FF;
				Packet->Data[1] = DEVICE_ID&0x00FF;
				Packet->Data[2] = (DEVICE_YEAR>>8)&0x00FF;
				Packet->Data[3] = DEVICE_YEAR&0x00FF;
				//
				Packet->Data[4] = (g_Ram.Status.VersionPO>>8)&0x00FF;
				Packet->Data[5] = g_Ram.Status.VersionPO&0x00FF;
				if (g_Ram.HideParam.Password1!=0) Packet->Data[6] = 1; else Packet->Data[6] = 0;
				if (g_Ram.HideParam.Password2!=0) Packet->Data[7] = 1; else Packet->Data[7] = 0;

				break;
			case MB_READ_REGS:
				switch(Res)
				{
					case 1:memcpy(Packet->Data, /*ToUnsPtr*/(&g_Ram.Status.Status) + Packet->Addr, Packet->Count);
							break;
					case 5:

						/*memcpy(Packet->Data, ToUnsPtr(&g_Stat.Im.WrBufferMb[0]), Packet->Count);
						g_Stat.Im.HardwareSrc=imSrcModbus;
						g_Stat.Im.AddrData = 0;
						g_Stat.Im.CanSendDataMb = 0;*/
						break;
					case 6:
						/*if (Addr==65125) g_Stat.Im.AddrData = g_Stat.Im.AddrData - Packet->Count;
						memcpy(Packet->Data, ToUnsPtr(&g_Stat.Im.WrBufferMb[g_Stat.Im.AddrData]), Packet->Count);
						g_Stat.Im.AddrData = g_Stat.Im.AddrData + Packet->Count;
						if (g_Stat.Im.AddrData>=g_Stat.Im.IndexMb)
						{
							g_Stat.Im.CanSendData = false;
							g_Stat.Im.CanSendDataMb = 0;
							g_Stat.Im.AddrData = 0;
							*g_Stat.Im.IsTxBusy = false;
						}*/
						break;
							
					default: return EX_ILLEGAL_FUNCTION;
				}
				break;
			case MB_WRITE_REGS:
				switch(Res)
				{
					case 1:
					/*	if (Addr>=REG_TASKCLOSE && Addr<=REG_RSRESET){
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							
						}
						if (Addr == REG_SET_DEFAULTS){
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							
						}
						if (Addr == REG_COM_REG)
						{
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							
						}*/
						return WriteData(Packet->Addr, Packet->Data, Packet->Count);
					case 5:
					/*	for(i=0; i<Packet->Count; i++)
						{
							g_Stat.Im.RdBuffer[i] = g_Comm.mbAsu.Frame.Buf[8+i*2];
						}

						g_Stat.Im.Index = Packet->Count;
						g_Stat.Im.HardwareSrc=imSrcModbus;
						g_Stat.Im.CanSendDataMb = 0;*/

						return 0;

					case 7:

						
						Packet->Request = 0;
						Packet->Response = 0;
					//	g_Stat.Im.RdBuffer[0]=0;
						
						for(i=0; i<Packet->Count; i++)
						{
							Packet->Data[i] = 0;
							//g_Comm.mbBt.Frame.Buf[i]=0;
						}
						Packet->Count = 0;


						return 0;
						//return WriteData(Packet->Addr, Packet->Data, Packet->Count);
					default: return EX_ILLEGAL_FUNCTION;
				}
				case MB_WRITE_REG:

					switch(Res)
							{
							case 1:
							/*if (Addr>=REG_TASKCLOSE && Addr<=REG_RSRESET){
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								
							}
							if (Addr == REG_SET_DEFAULTS){
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								
								//g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							}
							if (Addr == REG_COM_REG)
							{
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								
							}*/
							return WriteData(Packet->Addr, Packet->Data, 1);
							default: return EX_ILLEGAL_FUNCTION;
							}
			default: return EX_ILLEGAL_FUNCTION;
		}
		return 0;
}

//---------------------------------------------------
 Byte WriteData(Uns Addr, Uns *Data, Uns Count)
{
	struct MENU_DCR Dcr;
	struct MENU_VAL_CFG *Val = &Dcr.Config.Val;
	Uns  i, *Dest = ToUnsPtr(&g_Ram) + Addr;
	Bool Nvm = False;
	

	for (i=0; i < Count; i++)
	{
		GetDcr(Addr + i, &Dcr);
		if (Val->ReadOnly) {return EX_ILLEGAL_FUNCTION;}
		if (!CheckRange(Data[i], Dcr.Min, Dcr.Max)) 
			{return EX_ILLEGAL_DATA_VALUE;}
		if (Val->Memory)
		{
			Nvm = true;
		//	g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// Запомнили адрес параметра, инкрементировали индекс
		//	g_Stat.LogParam.MbIndex++;
		}
		if (Addr==REG_TASK_TIME || Addr==REG_TASK_DATE)
		{
		//	g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// Запомнили адрес параметра, инкрементировали индекс
		//	g_Stat.LogParam.MbIndex++;
		}
		/*if ((Addr >= REG_TASKCLOSE)&&(Addr <= REG_REVCLOSE))
		{
			g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// Запомнили адрес параметра, инкрементировали индекс
			g_Stat.LogParam.MbIndex++;
		}*/

	}

	
	if (Addr == REG_CODE){
		g_Core.VlvDrvCtrl.EvLog.Source = 0;
		if ( UpdateCode(REG_PASSW1_NEW, Addr, *Data, DEF_USER_PASS) )
			return FR_SUCCESS;
	}

	if (Addr == REG_FCODE){
		g_Core.VlvDrvCtrl.EvLog.Source = 0;
		if ( UpdateCode(REG_PASSW2_NEW, Addr, *Data, DEF_FACT_PASS) )
			return FR_SUCCESS;
	}
          
	if (Addr == REG_CONTROL)
	{
	
		if (!(g_Core.VlvDrvCtrl.ActiveControls & CMD_SRC_SERIAL) && (*Data != vcwStop))
			return EX_ILLEGAL_DATA_VALUE;
	}

	if (!menu.EnableEdit(Val->PaswwPrt))
	{
		return FR_SUCCESS;
	}

	if (Nvm && !IsMemParReady()) return EX_SLAVE_DEVICE_BUSY;

	memcpy(Dest, Data, Count);

	if (Nvm)
	{
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,Addr,Dest,Count))
		{
			RefreshParams(Addr);
			return FR_SUCCESS;
		}
	}

	return FR_SUCCESS;

}
//---------------------------------------------------







