/*======================================================================

========================================================================*/

#ifndef COMM_
#define COMM_

#include "std.h"
//#include "main.h"
#include "comm_ModbusRtu.h"
#include "comm_SerialComm.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ICREMENT_DIV 25		//
#define DECREMENT_DIV 10	//
//--------------------------------------------------------
//================== ������������ ========================
//--------------------------------------------------------

typedef struct {
	//TCommMPU		localControl;	// ��� ������ �����
	//----
	//TDigitalInput 	digitInput;	// ��� ������ ��
	// ---
	//TDigitalInterface	digitInterface;	// для работы с ТС
	// ---
	Uns			outputCmdReg;	// 
	// ---
	Uns			outputCmdSrc;	// 
	// ---
	TMbPort  		mbAsu;		// modbus ASU
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void Comm_50HzCalc(TComm *);
void Comm_CommandUpdate(TComm *p);
//Uns DigitCmdModeUpdate (Uns *);
void TekModbusParamsUpdate(void);
void comm_ControlModbusUpdatealtistar48(TComm *p);

extern TComm	                g_Comm;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
