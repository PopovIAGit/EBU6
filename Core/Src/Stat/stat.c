/*======================================================================
Имя файла:
Автор:
Версия файла:
Дата изменения:
======================================================================*/

#include "g_Ram.h"
#include "stat.h"
#include "g_Core.h"
//#include "comm.h"

TStat			g_Stat;
Uns 			prevCmd = 0, delayStart = 2.0 * PRD_50HZ;

char LogEvBufIndex = 0;
bool LogEvMainDataFlag = false;
Byte LogEvBufCurIndex = 0;
Byte LogEvBufSeconds = 0;
Bool ReadLogFlag = False;

//ma LogSIM 4 БЫЛО 3
Uns  PrevLogAddr[4] = {0,0,0,0};
Uns  PrevLogCount[4] = {0,0,0,0};



//---------------------------------------------------
// Инициализация журнала
void Stat_Init(TStat *s)
{
	int i=0;
}
//---------------------------------------------------

//---------------------------------------------------
// Обновление журнала
void Stat_Update(TStat *p)	// 50 Гц
{
	
}
//---------------------------------------------------
// Работа с Flash
//---------------------------------------------------
// Работа с Eeprom
Bool ReadWriteEeprom(pFM25V10 eeprom, Byte func, Uns addr, Uns *pData, Uns count)
{
	unsigned long CastAddr = (unsigned long)addr;

	if ((eeprom->Func)||(eeprom->Error)||(count < 1)) return false;

	eeprom->Addr	= CastAddr << 1;	// Сдвигаем адрес (умножаем на 2), т.к. данные в структуре 2 байта, а в память пишется побайтно
	eeprom->Buffer	= pData;		// Указатель на данные
	eeprom->Count	= count << 1;
	eeprom->Func	= func;

	return true;
}
//----------------------------------------------------

//---------Конец файла------------------------------------
