

#define _MASTER_
#define _SLAVE_

#include "comm_ModbusRtu.h"
#include "comm_ModbusConfig.h"
#include "comm_ModbusCrc.h"
#include "comm_ModbusEvent.h"
#include "comm_ModbusFrame.h"
#include "comm_ModbusInterrupts.h"

Uns testPreamble=0;
Uns TestCount;

Uns CrcTable[256];
static Bool CrcTableGenFlag = false;
static Bool OpenFlags[4] = {false, false, false, false};

__inline void UpdateNewFrame(TMbPort *hPort);

__inline void MasterRequest(TMbPort *hPort);
__inline void MasterConfirmation(TMbPort *hPort);

__inline void SlaveIndication(TMbPort *hPort);
__inline void SlaveResponse(TMbPort *hPort);

__inline void CrcPack(TMbPort *);


//-------------------------------------------------------------------------------
 void BreakFrameEvent(TMbPort *hPort)
{
	//if (hPort->Params.HardWareType==0) 
        
           //поменять функции  SCI_rx_disable(hPort->Params.ChannelID);	
}

//-------------------------------------------------------------------------------
 void NewFrameEvent(TMbPort *hPort)
{
	hPort->Frame.NewMessage = true;
	hPort->Frame.RxLength   = hPort->Frame.Data - hPort->Frame.Buf;
	hPort->Frame.Data       = hPort->Frame.Buf;
	if (hPort->Params.ChannelID == SCIB)
	{
		//hPort->Serial.RsState=1;
		hPort->Packet.Exception = FR_SUCCESS;
	}
}

//-------------------------------------------------------------------------------
 void PreambleEvent(TMbPort *hPort)
{
	hPort->Frame.Data = hPort->Frame.Buf;

	//поменять функцию if (hPort->Params.HardWareType==0) SCI_transmit(hPort->Params.ChannelID, *hPort->Frame.Data++);	
}

//-------------------------------------------------------------------------------
 void PostambleEvent(TMbPort *hPort)
{
	LgUns i=0;

	hPort->Frame.Data = hPort->Frame.Buf;
	hPort->Params.TrEnable(0);
	TestCount=0;

	if (hPort->Params.HardWareType==0){
		//for(i=0; i<1000; i++){}
		//SCI_tx_disable(hPort->Params.ChannelID);
		//SCI_rx_enable(hPort->Params.ChannelID);
	}
          
	if (hPort->Params.Mode==MB_SLAVE) hPort->Params.TrEnable(0);
	else if (hPort->Params.Mode==MB_MASTER)
	{
		StartTimer(&hPort->Frame.TimerAck);
	}
	testPreamble=0;
}

//-------------------------------------------------------------------------------
 void ConnTimeoutEvent(TMbPort *hPort)
{
	/*if (IsMaster())
	{
		if (hPort->Packet.Exception)
		{
			hPort->Stat.SlaveNoRespCount++;
			hPort->Frame.RetryCounter++;
			if (hPort->Frame.RetryCounter < hPort->Params.RetryCount)
			{
				hPort->Frame.WaitResponse = false;
				return;	
			}
			hPort->Packet.Exception = EX_NO_CONNECTION;
		}
		SendMasterResponse(hPort);
	}*/
	
	if(IsSlave())
	{
		if (hPort->Params.ChannelID==SCIB)
		{
			//hPort->Serial.RsState=EX_NO_CONNECTION;
			hPort->Packet.Exception = EX_NO_CONNECTION;
		}
	}
}

//-------------------------------------------------------------------------------
 void AcknoledgeEvent(TMbPort *hPort)
{
	if (hPort->Params.Mode==MB_SLAVE) hPort->Packet.Acknoledge = false;
	else if (hPort->Params.Mode==MB_MASTER)
	{
		if (hPort->Params.HardWareType==0){
			hPort->Params.TrEnable(1);
			//SCI_rx_disable(hPort->Params.ChannelID); поменять функцию
			//SCI_tx_disable(hPort->Params.ChannelID);

			if (hPort->Stat.SlaveNoRespCount<65500) hPort->Stat.SlaveNoRespCount++;// Счетчик неответов
			if (hPort->Frame.RetryCounter > hPort->Params.RetryCount)
			{
				//hPort->Packet.Exception = EX_NO_CONNECTION; //???
				hPort->Stat.Status.bit.NoConnect = 1;
			}
			StopTimer(&hPort->Frame.TimerConn);
			StopTimer(&hPort->Frame.TimerAck);
			if (hPort->Frame.RetryCounter<65500) hPort->Frame.RetryCounter++;
			hPort->Frame.WaitResponse = 0;
			hPort->Stat.Status.bit.Error = 1;

			//если времмя ожидания ответа истекло
			// то делаем N повторов
			// после последнего повтора выставляем статус нет связи
			// тут так же надо сбрасывать все флаги для возможности повтора
		}
		//запрещаем прием данных
		//смортим сколько повторов необходимо сделать
		//делаем повтор
		//если повторов не осталось то не делаем повтор
	}
}

//-------------------------------------------------------------------------------
void SendFrame(TMbPort *hPort)
{
	CrcPack(hPort);
	hPort->Params.TrEnable(1);

	/*if (hPort->Params.HardWareType==0)
	{
		
                 SCI_rx_disable(hPort->Params.ChannelID);
		SCI_tx_enable(hPort->Params.ChannelID);
	}*/

	StartTimer(&hPort->Frame.TimerPre);
	hPort->Stat.TxMsgCount++;
}

//-------------------------------------------------------------------------------
void SendMasterResponse(TMbPort *hPort)
{
	hPort->Packet.Response    = hPort->Packet.Request;
	hPort->Packet.Request     = 0;
	hPort->Frame.RetryCounter = 0;
	hPort->Frame.WaitResponse = false;
}


void ModBusInit(TMbPort *hPort)
{
	Bool *OpenFlag = &OpenFlags[hPort->Params.ChannelID];
	if (*OpenFlag == false)
	{
		memset(&hPort->Packet, 0, sizeof(TMbPacket));
		memset(&hPort->Frame,  0, sizeof(TMbFrame));
		hPort->Packet.ParamMode = hPort->Params.HardWareType;
		*OpenFlag = true;
	}

	if (!CrcTableGenFlag)
	{
		GenerateCrcTable();
		CrcTableGenFlag = true;
	}
	
	ResetCommumication(hPort, TRUE);
}

void ModBusInvoke(TMbPort *hPort)
{
	if (hPort->Frame.NewMessage)
	{
		hPort->Stat.MAMsgIn++;
		UpdateNewFrame(hPort);
		hPort->Frame.NewMessage = FALSE;
	}
	else
	{
		if (IsMaster()) MasterRequest(hPort);
		if (IsSlave())  SlaveResponse(hPort);
	}
}

void ModBusTimings(TMbPort *hPort)
{
	if (!TimerPending(&hPort->Frame.Timer1_5))
	{
		BreakFrameEvent(hPort);
	}
	if (!TimerPending(&hPort->Frame.Timer3_5))
	{
		NewFrameEvent(hPort);
	}
	if (!TimerPending(&hPort->Frame.TimerPre))
	{
		PreambleEvent(hPort);
	}
	if (!TimerPending(&hPort->Frame.TimerPost))
	{
		PostambleEvent(hPort);
	}
	if (!TimerPending(&hPort->Frame.TimerConn))
	{
		ConnTimeoutEvent(hPort);
	}
	if (!TimerPending(&hPort->Frame.TimerAck))
	{
		AcknoledgeEvent(hPort);
	}
}

__inline void UpdateNewFrame(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	TMbStat  *Stat  = &hPort->Stat;
	Byte Status;
	
	/* поменять функции if (hPort->Params.HardWareType==0) Status = SCI_getstatus(hPort->Params.ChannelID);  
	
	if (hPort->Params.HardWareType==0){
                        if (Status & SCI_RX_ERROR){
			if (Status & SCI_PARITY_ERROR)  Stat->BusParityErrCount++;
			if (Status & SCI_OVERRUN_ERROR) Stat->BusOverrunErrCount++;
			if (Status & SCI_FRAME_ERROR)   Stat->BusFrameErrCount++;
			SCI_reset(hPort->Params.ChannelID);
			goto FRAMING_ERROR;
		}
	} */
	
	if (Frame->RxLength < 3)
	{
		Stat->BusFrameLenErrCount++;
		goto FRAMING_ERROR;
	}	

	if (CalcFrameCrc(Frame->Buf, Frame->RxLength) != GOOD_CRC)
	{
		Stat->BusFrameCrcErrCount++;
		goto FRAMING_ERROR;
	}
	
	Stat->BusMsgCount++;
	
	if (IsMaster()) MasterConfirmation(hPort);
	if (IsSlave())  SlaveIndication(hPort);

	return;
	
FRAMING_ERROR:
	Stat->BusErrCount++;

	// поменять функции if (hPort->Params.HardWareType==0) SCI_rx_enable(hPort->Params.ChannelID);

	if (hPort->Frame.Buf[0]==0) hPort->Frame.Buf[0]=1;
	if (hPort->Frame.Buf[1]!=3 || hPort->Frame.Buf[1]!=16) hPort->Frame.Buf[1]=3;
	hPort->Packet.Exception = hPort->Frame.Buf[1];
	hPort->Packet.Response = EX_SLAVE_DEVICE_FAILURE;
}

__inline void MasterRequest(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;

	if (!Packet->Request) return;
	if (hPort->Frame.WaitResponse) return;
	
	if (hPort->Params.HardWareType==0)
		SCI_rx_disable(hPort->Params.ChannelID);	

	Packet->Exception = 0;
	switch(Packet->Request)
	{
		case 4:   ReadRegsRequest(hPort); break;
		case MB_READ_REGS:   ReadRegsRequest(hPort); break;
		case MB_WRITE_REG:   WriteRegRequest(hPort); break;
		case MB_WRITE_REGS:  WriteRegsRequest(hPort);break;
		default: Packet->Exception = EX_ILLEGAL_FUNCTION;
	}

	if (Packet->Exception || !hPort->Params.Slave)
		SendMasterResponse(hPort);//???
	else
	{
		hPort->Frame.WaitResponse = TRUE;
		StartTimer(&hPort->Frame.TimerConn);
		//if(LVS_flag ==3) LVS_flag = 1;
		//LVS_flag++;
		SendFrame(hPort);	
	}
}

__inline void SlaveIndication(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	Byte Slave, Func;
	
	Slave = hPort->Frame.Buf[0];

	/*поменять функции SCI_rx_enable if ((Slave != 0) && (Slave != hPort->Params.Slave))
	{
		if (hPort->Params.HardWareType==0) SCI_rx_enable(hPort->Params.ChannelID);
		

		return;	
	}*/
	
	hPort->Stat.SlaveMsgCount++;
	if (!Slave || hPort->Frame.ListenMode)
		hPort->Stat.SlaveNoRespCount++;
	
	Func = hPort->Frame.Buf[1];
	Packet->Exception = 0;
	switch(Func)
	{
		case MB_READ_REGS:   ReadRegsIndication(hPort); break;
		case MB_WRITE_REG:   WriteRegIndication(hPort); break;
		case MB_DIAGNOSTICS:
			switch(WordUnPack(&hPort->Frame.Buf[2]))
			{
				case MB_RET_QUERY_DATA:    ReturnQueryDataIndication(hPort);  break;
				case MB_RESTART_COMM:      RestartCommIndication(hPort);      break;
				case MB_RET_DIAGN_REG:     RetDiagnRegIndication(hPort);      break;
				case MB_FORCE_LISTEN:      ForceListenModeIndication(hPort);  break;
				case MB_CLEAR_DIAGN_REG:   ClearDiagnRegIndication(hPort);    break;
				case MB_RET_BUS_MSG:       RetBusMsgIndication(hPort);        break;
				case MB_RET_BUS_ERR:       RetBusErrIndication(hPort);        break;
				case MB_RET_BUS_EXCEPT:    RetBusExcIndication(hPort);        break;
				case MB_RET_SLAVE_MSG:     RetSlaveMsgIndication(hPort);      break;
				case MB_RET_SLAVE_NO_RESP: RetSlaveNoRespIndication(hPort);   break;
				case MB_RET_SLAVE_NAK:     RetSlaveNakIndication(hPort);      break;
				case MB_RET_SLAVE_BUSY:    RetSlaveBusyIndication(hPort);     break;
				case MB_RET_BUS_OVERRUN:   RetBusOverrunIndication(hPort);    break;
				case MB_CLEAR_OVERRUN:     ClearOverrunFlagIndication(hPort); break;
				default: Packet->Exception = EX_ILLEGAL_FUNCTION;
			}
		break;
		case MB_WRITE_REGS:  WriteRegsIndication(hPort);     break;
		case MB_REPORT_ID:   ReportSlaveIdIndication(hPort); break;
		default: Packet->Exception = EX_ILLEGAL_FUNCTION;
	}
	
	if (Packet->Exception || (Func == MB_DIAGNOSTICS)) Packet->Response = Func;
	else Packet->Request = Func;
}

__inline void SlaveResponse(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	TMbFrame *Frame = &hPort->Frame;
	
	if (!Packet->Response) return;
	
	StartTimer(&Frame->TimerConn);

	if (!Frame->Buf[0] || Frame->ListenMode)
	{
		Packet->Response = 0;
		StartTimer(&Frame->TimerPost);
		return;	
	}
	
	if (Packet->Exception)
	{
		hPort->Stat.BusExcCount++;
		switch(Packet->Exception)
		{
			case EX_ACKNOWLEDGE:          Packet->Acknoledge = TRUE; StartTimer(&Frame->TimerAck); break;
			case EX_SLAVE_DEVICE_BUSY:    hPort->Stat.SlaveBusyCount++; break;
			case EX_NEGATIVE_ACKNOWLEDGE: hPort->Stat.SlaveNakCount++;  break;
		}
		Frame->Buf[1]   = Packet->Response | 0x80;
		Frame->Buf[2]   = Packet->Exception;
		Frame->TxLength = 3;
		Packet->Exception = 0;
	}
	else switch(Packet->Response)
	{
		case MB_READ_REGS:  ReadRegsResponse(hPort);      break;
		case MB_WRITE_REG:  WriteRegResponse(hPort);      break;
		case MB_WRITE_REGS: WriteRegsResponse(hPort);     break;
		case MB_REPORT_ID:  ReportSlaveIdResponse(hPort); break;
		default: Packet->Response = 0; return;
	}
	Packet->Response = 0;
	hPort->Stat.MAMsgOut++;
	SendFrame(hPort);
}

__inline void MasterConfirmation(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	TMbFrame *Frame = &hPort->Frame;

	if ((Frame->Buf[1] & 0x7F) != Packet->Request)
	{
		Packet->Exception = FR_ERR_BAD_RESPONSE_FUNC;
	}
	else if (Frame->Buf[1] & 0x80)
	{
		if (Frame->RxLength != 5) Packet->Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		else
		{
			Packet->Exception = Frame->Buf[2];
			hPort->Stat.BusExcCount++;
			switch(Packet->Exception)
			{
				case EX_SLAVE_DEVICE_BUSY:    hPort->Stat.SlaveBusyCount++; break;
				case EX_NEGATIVE_ACKNOWLEDGE: hPort->Stat.SlaveNakCount++;  break;
			}
		}
	}
	else switch(Packet->Request)
	{
		case 4:   ReadRegsConfirmation(hPort); break;
		case MB_READ_REGS:   ReadRegsConfirmation(hPort); break;
		case MB_WRITE_REG:   WriteRegConfirmation(hPort); break;
		case MB_WRITE_REGS:  WriteRegsConfirmation(hPort);     break;
		case MB_REPORT_ID:   ReportSlaveIdConfirmation(hPort); break;
	}
	
	if (Packet->Exception) {

		if (hPort->Stat.BusErrCount<65500) hPort->Stat.BusErrCount++;
		if (hPort->Frame.RetryCounter > hPort->Params.RetryCount)
		{
			//hPort->Packet.Exception = EX_NO_CONNECTION; //???
			hPort->Stat.Status.bit.NoConnect = 1;
		}
		StopTimer(&Frame->TimerConn);
		StopTimer(&Frame->TimerAck);
		if (hPort->Frame.RetryCounter<65500) hPort->Frame.RetryCounter++;
		hPort->Frame.WaitResponse = 0;
		hPort->Stat.Status.bit.Error = 1;
	}
	else
	{
		hPort->Stat.Status.bit.Error = 0;
		hPort->Stat.Status.bit.Ready = 1;
		hPort->Stat.Status.bit.NoConnect = 0;
		hPort->Stat.SlaveMsgCount++;
		hPort->Frame.RetryCounter = 0;
		SendMasterResponse(hPort);
		StopTimer(&Frame->TimerConn);
		StopTimer(&Frame->TimerAck);
	}





}

//-------------------------------------------------------------------------------
void GenerateCrcTable(void)
{
	Uns i, j, Crc;
	for (i=0; i < 256; i++)
	{
		Crc = i;
		for (j=0; j < 8; j++)
		{
			if (Crc & 1) Crc = (Crc >> 1) ^ GENER_CRC;
			else Crc = (Crc >> 1);
		}
		CrcTable[i] = Crc;
	}
}

//-------------------------------------------------------------------------------
 Uns CalcFrameCrc(Byte *Buf, Uns Count)
{
	/*Uns Crc = INIT_CRC;
	do {Crc = (Crc >> 8) ^ CrcTable[(Crc ^ (Uns)*Buf++) & 0x00FF];}
	while (--Count);
	return Crc;*/
	Uns Crc = INIT_CRC;
	do {Crc = (Crc >> 8) ^ CrcTable[(Crc ^ (Uns)*Buf++) & 0x00FF];}
	while (--Count);
	return Crc;
}


//-------------------------------------------------------------------------------
 void WordPack(Byte *Buf, Uns Data)
{
	Buf[0] = (Byte)(Data >> 8);
	Buf[1] = (Byte)(Data & 0xFF);
}

 Uns WordUnPack(Byte *Buf)
{
	return (((Uns)Buf[0] << 8) | (Uns)Buf[1]);
}

 void DataPack(Byte *Buf, Uns *Data, Byte Count)
{
 	do {
		*Buf++ = (Byte)(*Data >> 8);
		*Buf++ = (Byte)(*Data & 0xFF);
		Data++;
	} while (--Count);
}

 void DataUnPack(Byte *Buf, Uns *Data, Byte Count)
{
 	do {
		*Data  = (Uns)(*Buf++) << 8;
		*Data |= (Uns)(*Buf++);
		Data++;
	} while (--Count);
}

__inline void CrcPack(TMbPort *hPort)
{
	Uns Crc = CalcFrameCrc((hPort->Frame.Buf), hPort->Frame.TxLength);
	hPort->Frame.Buf[hPort->Frame.TxLength++] = (Byte)(Crc & 0xFF);
	hPort->Frame.Buf[hPort->Frame.TxLength++] = (Byte)(Crc >> 8);	
}

//-------------------------------------------------------------------------------
 void DiagnosticRequest(TMbPort *hPort, Uns SubFunc, Bool Flag)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_DIAGNOSTICS;
	WordPack(&Buf[2], SubFunc);
	if (!Flag)
	{
		WordPack(&Buf[4], 0x0000);
		hPort->Frame.TxLength = 6;
	}
	else
	{
		DataPack(&Buf[4], hPort->Packet.Data, hPort->Packet.Count);
		hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 4;
	}
}

 void DiagnosticIndication(TMbPort *hPort, Uns Data)
{
	if ((hPort->Frame.RxLength != 8) || (WordUnPack(&hPort->Frame.Buf[4]) != 0x0000))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	WordPack(&hPort->Frame.Buf[4], Data);
	hPort->Frame.TxLength = 6;
}

 void DiagnosticConfirmation(TMbPort *hPort, Uns Count)
{
	if (hPort->Frame.RxLength != hPort->Frame.TxLength)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.SubRequest)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_FUNC;
		return;
	}
	DataUnPack(&hPort->Frame.Buf[4], hPort->Packet.Data, Count);
}

 void EchoResponse(TMbPort *hPort)
{
	hPort->Frame.TxLength = hPort->Frame.RxLength - 2;
}

//-------------------------------------------------------------------------------
 void ReadRegsRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_READ_REGS;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Count);
	hPort->Frame.TxLength = 6;
}

 void ReadRegsIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	hPort->Packet.Addr  = WordUnPack(&hPort->Frame.Buf[2]);
	hPort->Packet.Count = WordUnPack(&hPort->Frame.Buf[4]);
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
}

 void ReadRegsResponse(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[2] = hPort->Packet.Count << 1;
	DataPack(&Buf[3], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 3;
	/*Byte *Buf = hPort->Frame.Buf;
	//Buf[2] = hPort->Packet.Count << 1;
	DataPack(&Buf[2], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 3;*/
}

 void ReadRegsConfirmation(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (hPort->Frame.RxLength != (Buf[2] + 5))
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (Buf[2] != (hPort->Packet.Count << 1))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	DataUnPack(&Buf[3], hPort->Packet.Data, hPort->Packet.Count);

}

//-------------------------------------------------------------------------------
 void WriteRegRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_WRITE_REG;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Data[0]);
	hPort->Frame.TxLength = 6;
}

 void WriteRegIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	hPort->Packet.Addr    = WordUnPack(&hPort->Frame.Buf[2]);
	hPort->Packet.Data[0] = WordUnPack(&hPort->Frame.Buf[4]);
	hPort->Packet.Count   = 1;
}

 void WriteRegResponse(TMbPort *hPort)
{
	EchoResponse(hPort);
}

 void WriteRegConfirmation(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.Addr)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_ADDRESS;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[4]) != hPort->Packet.Data[0])
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_VALUE;
	}
}

//-------------------------------------------------------------------------------
 void ReturnQueryDataRequest(TMbPort *hPort)
{
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	DiagnosticRequest(hPort, MB_RET_QUERY_DATA, TRUE);
}

 void ReturnQueryDataIndication(TMbPort *hPort)
{
	EchoResponse(hPort);
}

 void ReturnQueryDataConfirmation(TMbPort *hPort)
{
	DiagnosticConfirmation(hPort, hPort->Packet.Count);
}

//-------------------------------------------------------------------------------
 void RestartCommRequest(TMbPort *hPort)
{
	if ((hPort->Packet.Data[0] != 0x0000) && (hPort->Packet.Data[0] != 0xFF00))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_VALUE;
		return;
	}
	hPort->Packet.Count = 1;
	DiagnosticRequest(hPort, MB_RESTART_COMM, TRUE);
}

 void RestartCommIndication(TMbPort *hPort)
{
	Uns Data;
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	Data = WordUnPack(&hPort->Frame.Buf[4]);
	if ((Data != 0x0000) && (Data != 0xFF00))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	ResetCommumication(hPort, Data != 0);
	EchoResponse(hPort);
}



 void ForceListenModeIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) hPort->Frame.ListenMode = TRUE;
}


 void ClearDiagnRegIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) memset(&hPort->Stat, 0, sizeof(TMbStat));
}

 void ClearOverrunFlagIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) hPort->Stat.BusOverrunErrCount = 0;
}


//-------------------------------------------------------------------------------
 void WriteRegsRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (!hPort->Packet.Count || (hPort->Packet.Count > 123))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_WRITE_REGS;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Count);
	Buf[6] = hPort->Packet.Count << 1;
	DataPack(&Buf[7], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = Buf[6] + 7;
}

 void WriteRegsIndication(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	hPort->Packet.Addr  = WordUnPack(&Buf[2]);
	hPort->Packet.Count = WordUnPack(&Buf[4]);
	if (!hPort->Packet.Count || (hPort->Packet.Count > 123))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	if (Buf[6] != (hPort->Packet.Count << 1)) //что за проверка???  Блиииять надоели эти сраные куски кода!!!
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	if (hPort->Frame.RxLength != (Buf[6] + 9))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	DataUnPack(&Buf[7], hPort->Packet.Data, hPort->Packet.Count);
}

 void WriteRegsResponse(TMbPort *hPort)
{
	hPort->Frame.TxLength = 6;
}

 void WriteRegsConfirmation(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.Addr)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_ADDRESS;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[4]) != hPort->Packet.Count)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
	}
}

//-------------------------------------------------------------------------------
 void ReportSlaveIdRequest(TMbPort *hPort)
{
	hPort->Frame.Buf[0] = hPort->Params.Slave;
	hPort->Frame.Buf[1] = MB_REPORT_ID;
	hPort->Frame.TxLength = 2;
}

 void ReportSlaveIdIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 4) hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
}

 void ReportSlaveIdResponse(TMbPort *hPort)
{
	Byte i, *Buf = hPort->Frame.Buf;
	Buf[2] = 8;//hPort->Packet.Count;
	for (i=0; i < Buf[2]; i++) Buf[i+3] = hPort->Packet.Data[i];
	/*Buf[3] = (DEVICE_ID>>8)&0x00FF;
	Buf[4] = DEVICE_ID&0x00FF;
	Buf[5] = (DEVICE_YEAR>>8)&0x00FF;
	Buf[6] = DEVICE_YEAR&0x00FF;
	//
	Buf[7] = (g_Ram.ramGroupA.VersionPO>>8)&0x00FF;
	Buf[8] = g_Ram.ramGroupA.VersionPO&0x00FF;
	if (g_Ram.ramGroupH.Password1!=0) Buf[9] = 1; else Buf[9] = 0;
	if (g_Ram.ramGroupH.Password2!=0) Buf[10] = 1; else Buf[10] = 0;*/

	hPort->Frame.TxLength = Buf[2] + 3;

}

 void ReportSlaveIdConfirmation(TMbPort *hPort)
{
	Byte i, *Buf = hPort->Frame.Buf;
	if (hPort->Frame.RxLength != (Buf[2] + 5))
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	for (i=0; i < Buf[2]; i++) hPort->Packet.Data[i] = Buf[i+3];
}


 void SetTimeout(TTimerList *Timer, Uns Timeout)
{
	if (!Timeout) Timeout = 1;
	Timer->Timeout = Timeout;
}

 void InitTimer(TTimerList *Timer, Uns Timeout)
{
	StopTimer(Timer);
	SetTimeout(Timer, Timeout);
}

 Bool TimerPending(TTimerList *Timer)
{
	if (Timer->Counter > 0)
	{
		Timer->Counter--;
		if (!Timer->Counter) return FALSE;
	}
	return TRUE;
}

void ModBusRxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Byte Data = 0;

	/*поменять функции if (hPort->Params.HardWareType==0)
	{
		Data = SCI_recieve(hPort->Params.ChannelID);
	}*/
		
	if ((Frame->Data - Frame->Buf) < 256)
	{
		
			*Frame->Data++ = Data;

		StartTimer(&Frame->Timer1_5);
		StartTimer(&Frame->Timer3_5);
	}

	hPort->Stat.RxBytesCount++;
}

//-------------------------------------------------------------------------------
void ModBusTxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Uns DataSend=0;
	Uns Stop=0;
	
	if ((Frame->Data - Frame->Buf) < Frame->TxLength){
		if (hPort->Params.HardWareType==0)
		{
			SCI_transmit(hPort->Params.ChannelID, *Frame->Data++);
			StartTimer(&Frame->TimerPost);
		}		
	}
	else
	{
		StartTimer(&Frame->TimerPost);
	}
	//StartTimer(&Frame->TimerPost);
	
	hPort->Stat.TxBytesCount++;
}

static void ResetCommumication(TMbPort *hPort, Bool ClearEventLog)
{
	TMbParams *Params = &hPort->Params;
	TMbFrame *Frame = &hPort->Frame;
	Uns Tout1_5, Tout3_5, Scale = (Uns)Params->Scale;
	
	// поменять функции if (hPort->Params.HardWareType==0) SCI_init(Params->ChannelID, Params->UartBaud, Params->Parity, 8);
	
	Params->TrEnable(0);
	
	StopTimer(&Frame->Timer1_5);
	StopTimer(&Frame->Timer3_5);
	StopTimer(&Frame->TimerPre);
	StopTimer(&Frame->TimerPost);
	#if defined(_SLAVE_)
	StopTimer(&Frame->TimerAck);
	#endif

	hPort->Packet.Request    = 0;
	hPort->Packet.Response   = 0;
	hPort->Packet.Exception  = EX_NO_CONNECTION;
	hPort->Packet.Acknoledge = false;
	
	Frame->ListenMode   = false;
	Frame->RetryCounter = 0;
	Frame->WaitResponse = false;
	Frame->NewMessage   = false;

	if (Params->BaudRate <= 192)
	{
		Tout1_5 = (165 * Scale) / Params->BaudRate; // 1.5 * 11 * 10 = 165
		Tout3_5 = (385 * Scale) / Params->BaudRate; // 3.5 * 11 * 10 = 385
	}
	else
	{
		Tout1_5 = (3 * Scale) >> 2; // 0.750
		Tout3_5 = (7 * Scale) >> 2; // 1.750
	}
	
	SetTimeout(&Frame->TimerPre,  Params->TxDelay * Scale);
	SetTimeout(&Frame->TimerPost, Tout3_5);
	SetTimeout(&Frame->TimerConn, Params->ConnTimeout * Scale);
	#if defined(_SLAVE_)
	SetTimeout(&Frame->TimerAck,  Params->AckTimeout * Scale);
	#endif
	
	Tout1_5 = Tout1_5 + Params->RxDelay;
	if (Tout3_5 < Tout1_5) Tout3_5 = Tout1_5 + 1;

	SetTimeout(&Frame->Timer1_5,  Tout1_5);
	SetTimeout(&Frame->Timer3_5,  Tout3_5);
	
	memset(&hPort->Stat, 0, sizeof(TMbStat));
	
	#if defined(_SLAVE_)
	if (IsSlave()) StartTimer(&Frame->TimerPost);
	#endif
}

