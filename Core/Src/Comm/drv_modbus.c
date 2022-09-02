
#include "main.h"
#include "stdint.h"
#include "std.h"
#define InRange(Val, Min, Max)	(((Val) >= (Min)) && ((Val) <= (Max)))
#define MB_SCALE		HZ

#define TOUT_1_5		(Uint16)(0.300 * MB_SCALE)
#define TOUT_3_5		(Uint16)(0.400 * MB_SCALE)
#define TOUT_WAIT		(Uint16)(0.000 * MB_SCALE)
#define TOUT_CONN		(Uint16)(3.000 * MB_SCALE)
#define TOUT_ACK		(Uint16)(1.000 * MB_SCALE)
//may habara
#define TOUT_PREAMBLE		(Uint16)(0.200 * MB_SCALE)
#define TOUT_POSTAMBLE		(Uint16)(0.200 * MB_SCALE)

#define TOUT_TESTSCI		(Uint16)(1.200 * MB_SCALE)


#define INIT_CRC		0xFFFF
#define GOOD_CRC		0x0000
#define GENER_CRC		0xA001



#define CHECK_TEK_MB_ADDR(Addr)	(((Addr>>8) & 0x9C) == 0x9C)


Uint16  TestSCI=0;

Bool MbConnect = false;
extern Bool PI_CalibEnable; 			// ���� ���������� ���������� ����� ��
Uns TempMbFlag = 0;

Uint16 CrcTable[256];
Uint16 MbTmpData[MB_DATA_MAX];
#define SCI_DEFAULT_BAUD_RATES { \
	24,48,96,192,384,576,1152 \
	}
static Uns BaudRates[7] = SCI_DEFAULT_BAUD_RATES;
Uint16 EnabledTransmit=0;

 void   ModBusSetup(TMbPort *, Uint16 UartID, TTrEnable TrEnable);
static void   ModBusReset(TMbPort *);
 void   ModBusRecieve(TMbPort *);
static Byte   ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count);
static Byte   WriteRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count);
//static void   ReadAckRegs(TMbPort *Port, Uint16 Addr, Uint16 Count);
//static void   WriteAckRegs(TMbPort *Port, Uint16 Addr, Uint16 Count);
void GenCrcTable(void);


 
 Uint16 CalcBufCRC(Uint16 *Buf, Uint16 Count);
 uint16_t CalcCRC(uint16_t CRCs, uint16_t c);
 Uint16 MaCalcTout(Uint16 Tout, Uint16 Baud);
void ReStartReadLine(TMbPort *Port);
 Uint16 CalcTout(Uint16 Tout, Uint16 Baud);
 Uint16 TimerPending(Uint16 *Timer);
 void   MbTxCtrl(Uint16 State);

void SerialCommInit(void)
{
	GenCrcTable();
	
	ModBusSetup(&g_Comm.Mb, 5, MbTxCtrl);
}

void ModBusControl(void)
{
	if (g_Ram.Comands.RsReset) {SerialCommRefresh(); g_Ram.Comands.RsReset = 0;}
}

void SerialCommUpdate(TMbPort *Port)
{
	if (PauseModbus > 0)
	{
	    return;
	}

	//if(TestSCI==1){
	//    return;
	//}

	MbConnect = Port->Frame.ConnFlg;	// ��������� ����� Modbus
	if (!Port->Frame.NewMsg) return;
	Port->Frame.NewMsg = 0;			// ����� switch'��, � �� �����
	ModBusRecieve(Port);
	//Port->Frame.Counter = 0;

	/*switch(Port->Frame.Flg3_5)
	{
		case 0:
			ModBusRecieve(Port);
			Port->Frame.Counter = 0;	// � ������ case �������� �������
			break;
		case 1:
			Port->Frame.Counter = 0;
			SCI_transmit(Port->Params.UartID, Port->Frame.Buf[0]);
			break;

		case 2:
			Port->Params.TrEnable(0);
			SCI_tx_disable(Port->Params.UartID);
			Port->Frame.Flg3_5 = 0;
			Port->Frame.Counter = 0;
			SCI_rx_enable(Port->Params.UartID);
			break;
	}*/
}
 Uns tmpdata1[10];
Uns cntfordata = 0;
void SerialCommRxHandler(TMbPort *Port)
{
	uint8_t Data;
	Port->Stat.RxBytesCount++;

	//Data = SCI_recieve(Port->Params.UartID);
        Data = UART5->RDR;
	if (Port->Frame.CounterForData < MB_FRAME_MAX)
	{
                cntfordata++;
                tmpdata1[cntfordata-1] = Data;
		Port->Frame.Buf[Port->Frame.CounterForData++] = Data;
                //Port->Frame.Buf[cntfordata] = Data;
		Port->Frame.Timer3_5 = Port->Params.Timeout3_5;
	}
}

void SerialCommTxHandler(TMbPort *Port)
{
	uint8_t Data;

		    Port->Stat.TxBytesCount++;

	    if (++Port->Frame.CounterForData < Port->Frame.Size)
	    {
		    Data =(uint8_t) Port->Frame.Buf[Port->Frame.CounterForData];
		    //SCI_transmit(Port->Params.UartID, Data);
		    HAL_UART_Transmit_IT(&huart5, &Data, 1);

                    
                    //todo may habara
		    //������� ������ ��� ����������� ���������� ��������
		    Port->Frame.TimerPost = Port->Params.TimeoutPost;
	    }
	
}

void SerialCommTimings(TMbPort *Port)
{
	

	if (!TimerPending(&Port->Frame.Timer3_5))
	{
	    //SCI_rx_disable(Port->Params.UartID);
	  __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);  
          Port->Frame.NewMsg = 1;
	}
	//may habara
	if (!TimerPending(&Port->Frame.TimerConn)) {Port->Frame.ConnFlg = 0; }
						    //Port->Frame.Exception = EX_NO_CONNECTION;}
	//may habara
	// ��������� ����� ��� ����������� ������ ��������
	if (!TimerPending(&Port->Frame.TimerPre))   {Port->Frame.CounterForData = 0;
				//SCI_transmit(Port->Params.UartID, Port->Frame.Buf[0]);
                    uint8_t tmp = Port->Frame.Buf[0];
                     HAL_UART_Transmit_IT(&huart5, &tmp, 1);           
        }
	// ���������� ���������� ��� ����������� ���������� ������ ���������� ��������
	if (!TimerPending(&Port->Frame.TimerPost))  {
						     //EnabledTransmit = 0;
						     HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)0);
	    		                             //SCI_tx_disable(Port->Params.UartID);
	    		                             //SCI_rx_enable(Port->Params.UartID);
	    		                             __HAL_UART_DISABLE_IT(&huart5, UART_IT_TXE);
                                                     __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
                                                     
                                                     Port->Frame.CounterForData = 0;
	}

}

void SerialCommRefresh(void)
{
	ModBusReset(&g_Comm.Mb);
}

 void ModBusSetup(TMbPort *Port, 
	Uint16 UartID, TTrEnable TrEnable)
{
	memset(&Port->Frame, 0, sizeof(TMbFrame));
	memset(&Port->Stat,  0, sizeof(TMbStat));

	Port->Params.UartID   = UartID;
	Port->Params.Slave    = g_Ram.UserParam.RsStation;
	Port->Params.BaudRate = BaudRates[g_Ram.UserParam.RsBaudRate]*100;
	Port->Params.Parity   = g_Ram.UserParam.RsMode;
	Port->Params.TrEnable = TrEnable;
}

static void ModBusReset(TMbPort *Port)
{
	Port->Params.Slave    = g_Ram.UserParam.RsStation;
	Port->Params.BaudRate = BaudRates[g_Ram.UserParam.RsBaudRate];
	Port->Params.Parity   = g_Ram.UserParam.RsMode;

	 HAL_UART_DeInit(&huart5);
             
           huart5.Instance = UART5;
            
           huart5.Init.BaudRate = Port->Params.BaudRate *100;
           
           huart5.Init.WordLength = UART_WORDLENGTH_8B;
           huart5.Init.StopBits = UART_STOPBITS_1;
           switch (Port->Params.Parity)
           {
             case pmNone: huart5.Init.Parity = UART_PARITY_NONE;  break;
             case pmOdd:  huart5.Init.Parity = UART_PARITY_ODD;   break;
             case pmEven: huart5.Init.Parity = UART_PARITY_EVEN;  break;
           }
             
           huart5.Init.Mode = UART_MODE_TX_RX;
           huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
           huart5.Init.OverSampling = UART_OVERSAMPLING_16;
           huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
           huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
           huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
            if (HAL_UART_Init(&huart5) != HAL_OK)
            {
              Error_Handler();
            }
            if (HAL_UARTEx_SetTxFifoThreshold(&huart5, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
            {
              Error_Handler();
            }
            if (HAL_UARTEx_SetRxFifoThreshold(&huart5, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
            {
              Error_Handler();
            }
            if (HAL_UARTEx_DisableFifoMode(&huart5) != HAL_OK)
            {
              Error_Handler();
            }
        

	
	Port->Params.Timeout1_5 = CalcTout(TOUT_1_5, Port->Params.BaudRate);
	Port->Params.Timeout3_5 = CalcTout(TOUT_3_5, Port->Params.BaudRate);
	Port->Params.TimeoutAck = TOUT_ACK;
	
	//may habara
	Port->Params.TimeoutPre = CalcTout(TOUT_PREAMBLE, Port->Params.BaudRate);
	Port->Params.TimeoutPost = CalcTout(TOUT_POSTAMBLE, Port->Params.BaudRate);
	Port->Params.TimeoutTestDelay = CalcTout(TOUT_TESTSCI, Port->Params.BaudRate);

	//may habara
	Port->Frame.NewMsg = 0;//may 1

	//Port->Params.TrEnable(0);
	HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)0);
        //SCI_tx_disable(Port->Params.UartID);
	__HAL_UART_DISABLE_IT(&huart5, UART_IT_TXE);
        Port->Frame.Flg3_5 = 0;
	Port->Frame.CounterForData = 0;
	//SCI_rx_enable(Port->Params.UartID);
        __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
	Port->Frame.TimerPre   = 0;
	Port->Frame.TimerPost  = 0;

	Port->Frame.Timer1_5   = 0;
	Port->Frame.Timer3_5   = 0;
	Port->Frame.TimerConn  = 0;
	Port->Frame.ConnFlg    = 0;
}

void ReStartReadLine(TMbPort *Port)
{
    //Port->Params.TrEnable(0);
  HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)0);  
  Port->Frame.Size = 0;
    Port->Frame.CounterForData = 0;
  //  SCI_tx_disable(Port->Params.UartID);
  //  SCI_rx_enable(Port->Params.UartID);
    
    __HAL_UART_DISABLE_IT(&huart5, UART_IT_TXE);
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
    Port->Frame.Timer3_5 = 0;
}

 void ModBusRecieve(TMbPort *Port)
{
	Uint16 Res, Func, Addr, Count;
	
	Port->Frame.Size = Port->Frame.CounterForData;
	
	
	Port->Frame.Crc = CalcBufCRC(Port->Frame.Buf, Port->Frame.Size);
	
	if ((Port->Frame.Buf[0] != Port->Params.Slave) ||
		 (Port->Frame.Crc != GOOD_CRC))
	{
		ReStartReadLine(Port);
		return;
	}
	
	Func  = Port->Frame.Buf[1];
	Addr  = (Port->Frame.Buf[2] << 8) | Port->Frame.Buf[3];
	Count = (Port->Frame.Buf[4] << 8) | Port->Frame.Buf[5];
	
	if (Port->Frame.Exception!=0)
	{	//test 485
	    Port->Frame.Exception = 0;
	}

	Port->Frame.Exception = 0;

	switch(Func)
	{
	    case MB_DIAGNOSTICS:
		if (Addr) Port->Frame.Exception = EX_ILLEGAL_FUNCTION;
		else Port->Frame.Size = Port->Frame.Size - 2;
	    break;
	    case MB_REP_SLAVE_ID:
	    break;
	    default:
		if (Port->Frame.Size < 5) { ReStartReadLine(Port); return;}
		if (!Count || (Count > MB_DATA_MAX)) {
		    Port->Frame.Exception = EX_ILLEGAL_DATA_VALUE; break;}
		Res = 1;

		
		if (!Res) {Port->Frame.Exception = EX_ILLEGAL_DATA_ADDRESS; break;}

		switch(Func)
		{
		    case MB_READ_REGS:
				switch(Res)
				{
					case 1: Port->Frame.Exception = ReadRegs(Port, (Uint16 *)&g_Ram, Addr, Count);
					break;
					default: Port->Frame.Exception = EX_ILLEGAL_FUNCTION;
				}
				break;

		    case MB_WRITE_REGS:
		    case MB_WRITE_REG:
				switch(Res)
				{
					case 1:
						if ( (Addr >= REG_TASK_CLOSE)&&(Addr <= REG_RS_RESET)) 	// ���� ���� ��������� �� ������ �� 210 �� 221 (������ D - ������� ������)
							   								// ��� �� ������ 3 (T4 ������� ������)
						{
							g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;						// �� ���������� �������� ������� RS-485
						}
						TempMbFlag = 1;
						Port->Frame.Exception = WriteRegs(Port, (Uint16 *)&g_Ram, Addr, Count);
						if (!Port->Frame.Exception) SerialCommRefresh();
						break;
					default: Port->Frame.Exception = EX_ILLEGAL_FUNCTION;
				}
				break;

		    default:
		    	Port->Frame.Exception = EX_ILLEGAL_FUNCTION;
		}
	}

	if (!Port->Params.Slave)
	{
	    ReStartReadLine(Port);
	    return;
	}
	else
	{
	    if (Port->Frame.Exception)
	    {
		Port->Frame.Buf[1] |= 0x80;
		Port->Frame.Buf[2]  = Port->Frame.Exception;
		Port->Frame.Size    = 3;
	    }
	    else
	    {
		Port->Frame.ConnFlg = 1;
		Port->Frame.TimerConn = TOUT_CONN;
	    }

	    Port->Frame.Crc = CalcBufCRC(Port->Frame.Buf, Port->Frame.Size);
	    Port->Frame.Buf[Port->Frame.Size++] = Port->Frame.Crc & 0xFF;
	    Port->Frame.Buf[Port->Frame.Size++] = Port->Frame.Crc >> 8;

	 //   SCI_rx_disable(Port->Params.UartID);
	  //  SCI_tx_enable(Port->Params.UartID);
            __HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
            __HAL_UART_ENABLE_IT(&huart5, UART_IT_TXE);
            
             Port->Frame.TimerPre = Port->Params.TimeoutPre;
             HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)1);
   
	}
}

static Byte ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count)
{
	Uint16 i, Tmp, *Buf;

	Buf = &Port->Frame.Buf[3];
	for (i=0; i < Count; i++)
	{
		Tmp = Data[Addr++];
		*Buf++ = Tmp >> 8;
		*Buf++ = Tmp & 0xFF;
	}
	
	Port->Frame.Buf[2] = Count << 1;
	Port->Frame.Size   = Port->Frame.Buf[2] + 3;

	return 0;
}

static Byte WriteRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count)
{
	struct MENU_DCR Dcr;
	struct MENU_VAL_CFG *Val = &Dcr.Config.Val;
	Uint16 i, Tmp, *Buf;
	Bool Nvm = False;

	// ��� ������� MB_WRITE_REGS - ���� Count �� ��������� - ������ ������
	if ((Port->Frame.Buf[6] != (Count << 1))&&(Port->Frame.Buf[1] == MB_WRITE_REGS))
	{
		return EX_ILLEGAL_DATA_VALUE;
	}
	// ��� ������� MB_WRITE_REGS - ���� ����� ������� �� ������ - ������ ������
	else if ((Port->Frame.Size != (Port->Frame.Buf[6] + 9))&&(Port->Frame.Buf[1] == MB_WRITE_REGS))
	{
		return EX_ILLEGAL_DATA_VALUE;
	}
	// ��� ������� MB_WRITE_REG - ���� ����� ������� �� ������ - ������ ������
	else if ((Port->Frame.Size != 8)&&(Port->Frame.Buf[1] == MB_WRITE_REG))
	{
		return EX_ILLEGAL_DATA_VALUE;
	}
	else
	{
		if (Port->Frame.Buf[1] == MB_WRITE_REGS)
			Buf = &Port->Frame.Buf[7];
		else
		{
			Buf = &Port->Frame.Buf[4];
			Count = 1;
		}

		for (i=0; i < Count; i++)
		{
			Tmp  = (*Buf++) << 8;
			Tmp |= (*Buf++) & 0xFF;
			GetDcr(i + Addr, &Dcr);
			if (Val->ReadOnly) {MbTmpData[i] = Data[i+Addr]; continue;}
			if (!CheckRange(Tmp, Dcr.Min, Dcr.Max))
				return EX_ILLEGAL_DATA_VALUE;
			if (Val->Memory)
			{
				Nvm = True;
				//LogParam.MbBuffer[LogParam.MbIndex] = i + Addr;		// ��������� ����� ���������, ���������������� ������
				//LogParam.MbIndex++;
			}
			
			MbTmpData[i] = Tmp;
		}
		Port->Frame.Size = 6;	// ???
	}
	

//	��������� ���� �� ������ ������� �� ����������������� ����������.
//	���� ���� ������, � ����� �� ��������, �� ������� ����������

	if (InRange(REG_CONTROL, Addr, Addr + (Count - 1)))
	{
		Tmp = MbTmpData[REG_CONTROL - Addr];
		// ���� ����� ����������, �� �� ���������� �������, ����� ������� ����
		if (!(g_Core.VlvDrvCtrl.ActiveControls & CMD_SRC_SERIAL) && (Tmp != 0) && Tmp != vcwStop)
			return EX_ILLEGAL_DATA_VALUE;
		if (Tmp == vcwStop) g_Core.VlvDrvCtrl.Tu.Ready = False;
	}

	if (Nvm && !IsMemParReady()) return EX_SLAVE_DEVICE_BUSY;

	memcpy(&Data[Addr], MbTmpData, Count);
	
	if (Nvm)
	{
		WritePar(Addr, &Data[Addr], Count);
		//RefreshData(Addr);

//	����� ����� ������������� ���������� �������� ��� �������
//	Mcu.EvLog.Value = CMD_PAR_CHANGE; ???
	}
	return 0;
}

 void GenCrcTable(void)
{
	Uint16 i, j, v;

	for (i=0; i < 256; i++)
	{
		v = i;
		for (j=0; j < 8; j++)
		{
			if (v & 1) v = (v >> 1) ^ GENER_CRC;
			else v = (v >> 1);
		}
		CrcTable[i] = v;
	}
}

uint16_t CalcCRC(uint16_t CRCs, uint16_t c)
{
        uint16_t tmp = ((CRCs >> 8) ^ CrcTable[(CRCs ^ c) & 0xFF]);
	return tmp;
}


 uint16_t CalcBufCRC(uint16_t *Buf, uint16_t Count)
{
	uint16_t CRCs = INIT_CRC;
	while(Count--) 
        {
          CRCs = CalcCRC(CRCs, *Buf++);
        } 
	return CRCs;
}


 uint16_t MaCalcTout(uint16_t Tout, uint16_t Baud)
{
	Tout = (uint16_t)(Tout / Baud);
	if (!Tout) return 1;
	return Tout;
}

 Uint16 CalcTout(Uint16 Tout, Uint16 Baud)
{
	Tout = (Uint16)Tout/ Baud;
	if (!Tout) return 1;
	return Tout;
}


 Uint16 TimerPending(Uint16 *Timer)
{
	if (*Timer > 0)
	{
		*Timer = *Timer - 1;
		if (*Timer == 0) return 0;
	}
	return 1;
}

 void MbTxCtrl(Uint16 State)
{
	HAL_GPIO_WritePin(RX485DE_BRP_GPIO_Port, RX485DE_BRP_Pin, (GPIO_PinState)State);
}

