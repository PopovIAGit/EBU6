/*======================================================================
Имя файла:          comm_SerialComm.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль ModBus
======================================================================*/

#ifndef SERIAL_COMM_
#define SERIAL_COMM_

// Подключение файлов

#include "g_Structs.h"
//#include "g_InitHardware.h"



#define SCI_BRR(Baud)		        (Uint16)((Uint32)(200e6/14 / 800)  / (Baud) - 1)
#define SCI_BRR20700(Baud)		(Uint16)((Uint32)(200e6/14 / 800)  / (Baud) - 1)
//---------------------------------------------------------------------------
#define SCI_DEFAULT_BAUD_RATES { \
	24,48,96,192,384,576,1152 \
	}

#define SCI_DEFAULT_BRR_VALUES { \
	SCI_BRR(24),SCI_BRR(48),SCI_BRR(96),SCI_BRR(192),SCI_BRR(384),SCI_BRR(576),SCI_BRR(1152) \
	}

extern Bool MbAsuConnect;


void AsuMbSetTr(Byte Lev);


#endif

