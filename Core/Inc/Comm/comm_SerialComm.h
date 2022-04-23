/*======================================================================
��� �����:          comm_SerialComm.h
�����:
������ �����:
���� ���������:
��������:
������ ModBus
======================================================================*/

#ifndef SERIAL_COMM_
#define SERIAL_COMM_

// ����������� ������

#include "g_Structs.h"
//#include "g_InitHardware.h"



#define SCI_BRR(Baud)		        (Uint16)((Uint32)(150e6/14 / 800)  / (Baud) - 1)
#define SCI_BRR20700(Baud)		(Uint16)((Uint32)(150e6/14 / 800)  / (Baud) - 1)
//---------------------------------------------------------------------------
#define SCI_DEFAULT_BAUD_RATES { \
	24,48,96,192,384,576,1152 \
	}

#define SCI_DEFAULT_BRR_VALUES { \
	SCI_BRR(24),SCI_BRR(48),SCI_BRR(96),SCI_BRR(192),SCI_BRR(384),SCI_BRR(576),SCI_BRR(1152) \
	}


extern Bool MbBkpConnect;
extern Bool MbAsuConnect;
extern Bool MbShnConnect;
extern Bool MbBtConnect;

//__inline void AsuMbSetTr(Byte Lev)  {RS485_DIR_ASU = Lev;}
//__inline void ShnMbSetTr(Byte Lev)  {RS485_DIR_SHN = Lev;}




#endif

