/*======================================================================
��� �����:          modbus_rtu.h
�����:              ������ �.�.
������ �����:       01.03
���� ���������:		19/02/10
�������������:      ��������� � ����������� CSL
��������:
������� ������������ �� ��������� ModBus RTU
======================================================================*/

/*#ifndef MODBUS_RTU_
#define MODBUS_RTU_

#include "responses.h"

#ifdef __cplusplus
extern "C" {
#endif

// ������ ������
#define MB_SLAVE             0        // ����� �������� ����������
#define MB_MASTER            1        // ����� �������� ����������

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
	Byte UartID;               // ������������� UART-�����
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
} TMbParams;

// �������� ������ ������
typedef struct _TMbPacket {
	Byte Request;              // ��� ������� �������
	Byte Response;             // ��� ������� ������
	Uns  SubRequest;           // ��� ���������� �������
	Uns  Addr;                 // ��������� ����� ������
	Uns  Data[125];            // ����� ������
	Uns  Count;                // ���������� ������
	Byte Exception;            // ��� ����������
	Bool Acknoledge;           // ���� �������� �������������
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
	Byte      *Data;           // ��������� � ������ ������ ������
	Byte       Buf[256];       // ����� ������ �����
} TMbFrame;

// ��������� ���������� ������
typedef struct _TMbStat {
	Uns DiagnReg;              // ������� �����������
	Uns BusMsgCount;           // ������� ���������
	Uns BusErrCount;           // ������� ������ �����
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
} TMbStat;

// ��������� ������� ��������
typedef struct _TMbPort {
	TMbParams Params;          // ��������� ��������
	TMbPacket Packet;          // ����� ������
	TMbFrame  Frame;           // ��������� �����
	TMbStat   Stat;            // ���������� ������
} TMbPort;

typedef struct _TMbPort *TMbHandle;

// ��������� �������
void ModBusInit(TMbHandle);
void ModBusInvoke(TMbHandle);
void ModBusTimings(TMbHandle);
void ModBusRxIsr(TMbHandle);
void ModBusTxIsr(TMbHandle);

#ifdef __cplusplus
}
#endif // extern "C"

#endif*/



