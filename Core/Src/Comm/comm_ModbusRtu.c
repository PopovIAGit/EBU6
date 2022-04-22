

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


static Bool CrcTableGenFlag = false;
static Bool OpenFlags[4] = {false, false, false, false};

__inline void UpdateNewFrame(TMbPort *hPort);

__inline void MasterRequest(TMbPort *hPort);
__inline void MasterConfirmation(TMbPort *hPort);

__inline void SlaveIndication(TMbPort *hPort);
__inline void SlaveResponse(TMbPort *hPort);





__inline void CrcPack(TMbPort *);


//-------------------------------------------------------------------------------
__inline void BreakFrameEvent(TMbPort *hPort)
{
	if (hPort->Params.HardWareType==0) SCI_rx_disable(hPort->Params.ChannelID);	
}

//-------------------------------------------------------------------------------
__inline void NewFrameEvent(TMbPort *hPort)
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
__inline void PreambleEvent(TMbPort *hPort)
{
	hPort->Frame.Data = hPort->Frame.Buf;

	if (hPort->Params.HardWareType==0) SCI_transmit(hPort->Params.ChannelID, *hPort->Frame.Data++);	
}

//-------------------------------------------------------------------------------
__inline void PostambleEvent(TMbPort *hPort)
{
	LgUns i=0;

	hPort->Frame.Data = hPort->Frame.Buf;
	hPort->Params.TrEnable(0);
	TestCount=0;

	if (hPort->Params.HardWareType==0){
		//for(i=0; i<1000; i++){}
		SCI_tx_disable(hPort->Params.ChannelID);
		SCI_rx_enable(hPort->Params.ChannelID);
	}
          
	if (hPort->Params.Mode==MB_SLAVE) hPort->Params.TrEnable(0);
	else if (hPort->Params.Mode==MB_MASTER)
	{
		StartTimer(&hPort->Frame.TimerAck);
	}
	testPreamble=0;
}

//-------------------------------------------------------------------------------
__inline void ConnTimeoutEvent(TMbPort *hPort)
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
__inline void AcknoledgeEvent(TMbPort *hPort)
{
	if (hPort->Params.Mode==MB_SLAVE) hPort->Packet.Acknoledge = false;
	else if (hPort->Params.Mode==MB_MASTER)
	{
		if (hPort->Params.HardWareType==0){
			hPort->Params.TrEnable(1);
			SCI_rx_disable(hPort->Params.ChannelID);
			SCI_tx_disable(hPort->Params.ChannelID);

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

	if (hPort->Params.HardWareType==0)
	{
		SCI_rx_disable(hPort->Params.ChannelID);
		SCI_tx_enable(hPort->Params.ChannelID);
	}

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
	
	if (hPort->Params.HardWareType==0) Status = SCI_getstatus(hPort->Params.ChannelID);
	
	if (hPort->Params.HardWareType==0){
                        if (Status & SCI_RX_ERROR){
			if (Status & SCI_PARITY_ERROR)  Stat->BusParityErrCount++;
			if (Status & SCI_OVERRUN_ERROR) Stat->BusOverrunErrCount++;
			if (Status & SCI_FRAME_ERROR)   Stat->BusFrameErrCount++;
			SCI_reset(hPort->Params.ChannelID);
			goto FRAMING_ERROR;
		}
	} 
	
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

	if (hPort->Params.HardWareType==0) SCI_rx_enable(hPort->Params.ChannelID);

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

	if ((Slave != 0) && (Slave != hPort->Params.Slave))
	{
		if (hPort->Params.HardWareType==0) SCI_rx_enable(hPort->Params.ChannelID);
		

		return;	
	}
	
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



