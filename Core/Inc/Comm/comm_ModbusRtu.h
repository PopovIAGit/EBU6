/*======================================================================

======================================================================*/

#ifndef MODBUS_RTU_
#define MODBUS_RTU_


#include "comm_ModBusTimers.h"
#include "g_Structs.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  
#define INIT_CRC    0xFFFF
#define GOOD_CRC    0x0000
#define GENER_CRC   0xA001
  
  // ����� ������ ��� ����� � �� � ���
#define SCIA	0
#define SCIB	1
#define SCIC	2



#define ASU_SCI_ID			SCIB


#define SCI_PARITY_ERROR	0x04
#define SCI_OVERRUN_ERROR	0x08
#define SCI_FRAME_ERROR		0x10
#define SCI_RX_ERROR		0x1C

#define FR_SUCCESS                       0x00
#define FR_ERR_BAD_PARAMS                0x0C
#define FR_ERR_NOT_RESPOND               0x0E
#define FR_ERR_FUNC_NOT_SUPPORTED        0x0F
#define FR_ERR_CRC_FAILED                0x11
#define FR_ERR_BAD_RESPONSE_LENGTH       0x12
#define FR_ERR_BAD_RESPONSE_ADDRESS      0x13
#define FR_ERR_BAD_RESPONSE_FUNC         0x14
#define FR_ERR_BAD_DATA_LENGTH           0x15
#define FR_ERR_BAD_DATA_ADDRESS          0x16
#define FR_ERR_BAD_DATA_NUM              0x17
#define FR_ERR_BAD_DATA_VALUE            0x18
#define FR_ERR_BAD_EXCERTION_RESPONSE    0x19

#define EX_ILLEGAL_FUNCTION              0x01
#define EX_ILLEGAL_DATA_ADDRESS          0x02
#define EX_ILLEGAL_DATA_VALUE            0x03
#define EX_SLAVE_DEVICE_FAILURE          0x04
#define EX_ACKNOWLEDGE                   0x05
#define EX_SLAVE_DEVICE_BUSY             0x06
#define EX_NEGATIVE_ACKNOWLEDGE          0x07
#define EX_MEMORY_PARITY_ERROR           0x08
#define EX_NO_CONNECTION                 0x09

#define IsMaster()	(hPort->Params.Mode == MB_MASTER)
#define IsSlave()	(hPort->Params.Mode == MB_SLAVE)

// ������ ������
#define MB_SLAVE             0        // ����� �������� ����������
#define MB_MASTER            1        // ����� �������� ����������

// serial_communication
#define MB_SCALE			 (0.001 * PRD_2KHZ)	// ������������� ��� ����� �� ��������� ModBus

// �������������� �������
#define MB_READ_REGS         0x03     // ������ ���������
#define MB_WRITE_REG         0x06     // ������ ��������
#define MB_DIAGNOSTICS       0x08     // �����������
#define MB_WRITE_REGS        0x10     // ������ ���������
#define MB_REPORT_ID         0x11     // ������ ��������������� ����������

// �������������� ���������� ��� �����������
#define MB_RET_QUERY_DATA    0x0000   // ������� ������������� ������
#define MB_RESTART_COMM      0x0001   // ����� ������������
#define MB_RET_DIAGN_REG     0x0002   // ������ �������� �����������
#define MB_FORCE_LISTEN      0x0004   // ������� � ����� "Listen Mode Only"
#define MB_CLEAR_DIAGN_REG   0x000A   // ����� ��������� � �������� �����������
#define MB_RET_BUS_MSG       0x000B   // ������ �������� ���������
#define MB_RET_BUS_ERR       0x000C   // ������ �������� ������ ����� (CRC)
#define MB_RET_BUS_EXCEPT    0x000D   // ������ �������� ����������
#define MB_RET_SLAVE_MSG     0x000E   // ������ �������� ������������ ���������
#define MB_RET_SLAVE_NO_RESP 0x000F   // ������ �������� ���������
#define MB_RET_SLAVE_NAK     0x0010   // ������ �������� ������������� �������������
#define MB_RET_SLAVE_BUSY    0x0011   // ������ �������� ��������� ����������
#define MB_RET_BUS_OVERRUN   0x0012   // ������ �������� ��������� ������
#define MB_CLEAR_OVERRUN     0x0014   // ������� �������� ��������� � ����� ������
  
  
typedef struct _TTimerList {Uns Counter, Timeout;} TTimerList;

#define StopTimer(Timer)	(Timer)->Counter = 0
#define StartTimer(Timer)	(Timer)->Counter = (Timer)->Timeout

#define RestartCommConfirmation(hPort)		DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define RetDiagnRegRequest(hPort)				DiagnosticRequest(hPort, MB_RET_DIAGN_REG, FALSE)
#define RetDiagnRegIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.DiagnReg)
#define RetDiagnRegConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define ForceListenModeRequest(hPort)			DiagnosticRequest(hPort, MB_FORCE_LISTEN, FALSE)

#define ForceListenModeConfirmation(hPort)	DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define ClearDiagnRegRequest(hPort)			DiagnosticRequest(hPort, MB_CLEAR_DIAGN_REG, FALSE) 
  
  
#define ClearDiagnRegConfirmation(hPort)		DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define RetBusMsgRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_MSG, FALSE)
#define RetBusMsgIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusMsgCount)
#define RetBusMsgConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusErrRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_ERR, FALSE)
#define RetBusErrIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusErrCount)
#define RetBusErrConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusExcRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_EXCEPT, FALSE)
#define RetBusExcIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusExcCount)
#define RetBusExcConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveMsgRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_MSG, FALSE)
#define RetSlaveMsgIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveMsgCount)
#define RetSlaveMsgConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveNoRespRequest(hPort)			DiagnosticRequest(hPort, MB_RET_SLAVE_NO_RESP, FALSE)
#define RetSlaveNoRespIndication(hPort)		DiagnosticIndication(hPort, hPort->Stat.SlaveNoRespCount)
#define RetSlaveNoRespConfirmation(hPort)	DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveNakRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_NAK, FALSE)
#define RetSlaveNakIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveNakCount)
#define RetSlaveNakConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveBusyRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_BUSY, FALSE)
#define RetSlaveBusyIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveBusyCount)
#define RetSlaveBusyConfirmation(hPort)		        DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusOverrunRequest(hPort)			DiagnosticRequest(hPort, MB_RET_BUS_OVERRUN, FALSE)
#define RetBusOverrunIndication(hPort)		        DiagnosticIndication(hPort, hPort->Stat.BusOverrunErrCount)
#define RetBusOverrunConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define ClearOverrunFlagRequest(hPort)		DiagnosticRequest(hPort, MB_CLEAR_OVERRUN, FALSE)

#define ClearOverrunFlagConfirmation(hPort)	DiagnosticConfirmation(hPort, 0)
  
// ��������� ���������� ��������
typedef void (*TMbTrFunc)(Byte);

typedef struct _TMbParams {
	Byte ChannelID;            // ������������� UART-�����
	Byte Mode;                 // ����� ������
	Byte Slave;                // ����� ������������ ����������
	Uns  BaudRate;             // �������� ������ / 100
	Uns  UartBaud;             // ����������� �������� ��� UART
	Byte Parity;               // ����� ��������
	Byte RetryCount;           // ���������� �������� ������� (� ������ MASTER)
	Byte Scale;                // ������� ������ ��������� / 1000
	Uns  RxDelay;              // �������� ��� ������ ������ (� ������)
	Uns  TxDelay;              // �������� ��� �������� ������ (� ����)
	Uns  ConnTimeout;          // ����-��� ����������� ����� (� ����)
	Uns  AckTimeout;           // ����-��� ������������� (� ����)
	TMbTrFunc TrEnable;   	   // ������� ���������� ��������
	Uns  HardWareType;			// ��� ����������� ������ �������� ������
} TMbParams;

// �������� ������ ������
typedef struct _TMbPacket {
	Byte Request;              // ��� ������� �������
	Byte Response;             // ��� ������� ������
	Uns  SubRequest;           // ��� ���������� �������
	Uns  Addr;                 // ��������� ����� ������
	Uns  Data[100];             // ����� ������
	Uns  Count;                // ���������� ������
	Byte Exception;            // ��� ����������
	Bool Acknoledge;           // ���� �������� �������������
	Byte ParamMode;			   // ����� Modbus / Bluetooth
} TMbPacket;

// ��������� �����
typedef struct _TMbFrame {
	Bool       ListenMode;     // ������� ���������� � ������ "Listen Mode Only"
	Byte       RetryCounter;   // ���������� �������� ������� (� ������ MASTER)
	Bool       WaitResponse;   // ���� �������� ������ (� ������ MASTER)
	Bool       NewMessage;     // ���� ������ ������ �����
	TTimerList Timer1_5;       // ������ ��� 1.5 �������
	TTimerList Timer3_5;       // ������ ��� 3.5 �������
	TTimerList TimerPre;       // ������ ��� ���������
	TTimerList TimerPost;      // ������ ��� ����������
	TTimerList TimerConn;      // ������ ��� ����������� ������� �����
	TTimerList TimerAck;       // ������ ��� ��� ������������
	Uns        RxLength;       // ����� ��������� �����
	Uns        TxLength;       // ����� ������������� �����
	Uns 	   AddCount;
	Byte      *Data;           // ��������� � ������ ������ ������
	Byte       Buf[256];       // ����� ������ �����
} TMbFrame;

typedef union {
	Uint16 all;
	struct {
		Uint16 Busy:1;		// 0 - ����� ���� ���������� ��������� �������
		Uint16 Ready:1;		// 1 - ����� ������� ���������
		Uint16 Wait:1;		// 2 - �������� �������
		Uint16 NoConnect:1; // 3 - ��� �����
		Uint16 Error:1;		// 4 - ������ ��� ���������� �������
		Uint16 Rsvd:11;		// 5-15
	} bit;
} TATS48_mbStatus;

// ��������� ���������� ������
typedef struct _TMbStat {
	Uns DiagnReg;              // ������� �����������
	Uns BusMsgCount;           // ������� ���������
	Uns BusErrCount;           // ������� ������ �����
	Uns SyncTxErrCount;			// ������� ������ ����� �� ����
	Uns SyncRxErrCount;			// ������� ������ ����� �� ��������
	Uns BusExcCount;           // ������� ����������
	Uns SlaveMsgCount;         // ������� ������������ ���������
	Uns SlaveNoRespCount;      // ������� ���������
	Uns SlaveNakCount;         // ������� ������������� �������������
	Uns SlaveBusyCount;        // ������� ��������� ����������
	Uns BusParityErrCount;		// ������� ������ ���� ��������
	Uns BusOverrunErrCount;    // ������� ��������� ������
	Uns BusFrameErrCount;		// ������� ������ ���������� ��������� ����
	Uns BusFrameLenErrCount;	// ������� ������ ������ ������� �������� ������
	Uns BusFrameCrcErrCount;	// ������� ������ CRC �����
	Uns TxMsgCount;            // ������� ���������� ���������
	Uns RxBytesCount;          // �������� ���������� ����
	Uns TxBytesCount;          // ����������� ���������� ����
	Uns MAMsgIn;
	Uns MAMsgOut;	
        
        TATS48_mbStatus Status;	   // ������ ���������� �������
} TMbStat;

typedef struct _TSerialInterface
{
	Uns            RsState;             // 0.��������� �����
	TBaudRate      RsBaudRate;          // 1.�������� �����
	Uns            RsStation;           // 2.����� �������
	TParityMode    RsMode;              // 3.����� �����
	Uns            RsReset;             // 4.����� �����
	Uns            RsRxErrors;          // 6.���������� ������ ������
	Uns            RsIndicTime;         // 7.����� ����������� �����
	Uns            RsWaitTime;          // 8.����� �������� ��������
	Uns            Rsvd1;             	// 9.������
	Uns            PduRead;             // 10.������ ������ � ������ ���
	Uns            PduWrite;            // 11.������ ������ �� ������ ���
} TSerialInterface;

// ��������� ������� ��������
typedef struct _TMbPort {
	TMbParams Params;               // ��������� ��������
	TMbPacket Packet;               // ����� ������
	TMbFrame  Frame;                // ��������� �����
	TMbStat   Stat;                 // ���������� ������
	TSerialInterface Serial;	// ������. ���������

	/*void (*HardwareSetup)(TMbHandle);
	Bool (*CheckCommError)(void);
	void (*EnableRx)(void);
	void (*EnableTx)(void);
	Byte (*ReceiveByte)(void);
	void (*TransmitByte)(Byte Data);*/

} TMbPort,*TMbHandle;

//typedef struct TMbPort ;

// ��������� �������
void ModBusInit(TMbHandle);
void ModBusInvoke(TMbHandle);
void ModBusTimings(TMbHandle);
void ModBusRxIsr(TMbHandle);
void ModBusTxIsr(TMbHandle);
void SendMasterResponse(TMbPort *);
void SendFrame(TMbPort *);
void GenerateCrcTable(void);
 Uns CalcFrameCrc(Byte *Buf, Uns Count);

// ��������� �������
void SerialCommInit(TMbHandle);
void ModBusSlaveReset(TMbHandle);
void SerialCommTimings(TMbHandle);
void InitChanelAsuModbus(TMbHandle);
void InitChanelEncoderModbus(TMbHandle);
void ModBusUpdate(TMbHandle);

 void ResetCommumication(TMbPort *, Bool);
  void BreakFrameEvent(TMbPort *hPort);
  void NewFrameEvent(TMbPort *hPort);
   void PreambleEvent(TMbPort *hPort);
    void PostambleEvent(TMbPort *hPort);
     void ConnTimeoutEvent(TMbPort *hPort);
      void AcknoledgeEvent(TMbPort *hPort);
  Uns WordUnPack(Byte *Buf);
   void DiagnosticIndication(TMbPort *hPort, Uns Data);
    void ReadRegsRequest(TMbPort *hPort);
    void ReadRegsIndication(TMbPort *hPort);
    void ReadRegsResponse(TMbPort *hPort);
     void ReadRegsConfirmation(TMbPort *hPort);
     void WriteRegRequest(TMbPort *hPort);
     void WriteRegIndication(TMbPort *hPort);
     void WriteRegResponse(TMbPort *hPort);
     void WriteRegConfirmation(TMbPort *hPort);
     void ReturnQueryDataIndication(TMbPort *hPort);
     void RestartCommIndication(TMbPort *hPort);
      void ForceListenModeIndication(TMbPort *hPort);
       void ClearDiagnRegIndication(TMbPort *hPort);
       void ClearOverrunFlagIndication(TMbPort *hPort);
       void WriteRegsRequest(TMbPort *hPort);
       void WriteRegsIndication(TMbPort *hPort);
       void WriteRegsResponse(TMbPort *hPort);
       void WriteRegsConfirmation(TMbPort *hPort);
       void ReportSlaveIdIndication(TMbPort *hPort);
        void ReportSlaveIdResponse(TMbPort *hPort);
        void ReportSlaveIdConfirmation(TMbPort *hPort);
                                    
 void SetTimeout(TTimerList *Timer, Uns Timeout);
 void InitTimer(TTimerList *Timer, Uns Timeout);
 Bool TimerPending(TTimerList *Timer);

#ifdef __cplusplus
}
#endif // extern "C"

#endif



