/*======================================================================

======================================================================*/

#ifndef MODBUS_RTU_
#define MODBUS_RTU_


#include "comm_ModBusTimers.h"
#include "g_Structs.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  
  
  // ����� ������ ��� ����� � �� � ���
#define SCIA	0
#define SCIB	1
#define SCIC	2


#define BKP_SCI_ID			SCIA
#define ASU_SCI_ID			SCIB
#define SHN_SCI_ID			SCIC

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
	TMbTrFunc TrEnable;   		// ������� ���������� ��������
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


// ��������� �������
void SerialCommInit(TMbHandle);
void ModBusSlaveReset(TMbHandle);
void SerialCommTimings(TMbHandle);
void InitChanelAsuModbus(TMbHandle);
void InitChanelEncoderModbus(TMbHandle);
void ModBusUpdate(TMbHandle);

#ifdef __cplusplus
}
#endif // extern "C"

#endif



