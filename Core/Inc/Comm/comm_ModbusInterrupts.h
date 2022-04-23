/*======================================================================
Имя файла:          modbus_interrupts.h
Автор:              
Версия файла:       01.00
Описание:
Модуль обработки прерываний приема и передачи
======================================================================*/

#ifndef MODBUS_INTERRUPTS_
#define MODBUS_INTERRUPTS_

//-------------------------------------------------------------------------------
void ModBusRxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Byte Data = 0;

	if (hPort->Params.HardWareType==0)
	{
		Data = SCI_recieve(hPort->Params.ChannelID);
	}
		
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

#endif

