#ifndef _FM25V10_H_
#define _FM25V10_H_

#include "std.h"

// Биты статусного регистра
#define FM25V10_WEL    0x02
#define FM25V10_BP0    0x04
#define FM25V10_BP1    0x08
#define FM25V10_WPEN   0x80

#define EEPROM_RETRY			(5)					// Количество попыток перезаписи в EEPROM

//#define IsEepromReady()		(!g_Eeprom.func)

// Макросы флеш
#define IsMemParReady()				(!Eeprom1.Func)
#define IsMemLogReady()				(!Eeprom2.Func)

#define WAIT_FOR_EEPROM_READY() while (!IsMemParReady())	{FM25V10_Update(&Eeprom1);

#define ReadFromEeprom(addr,buf,count)	ReadWriteEeprom(&Eeprom1,F_READ,addr,buf,count)
#define WriteToEeprom(addr,buf,count)	ReadWriteEeprom(&Eeprom1,F_WRITE,addr,buf,count)

// Структура параметров драйвера
typedef void (*TCsFunc)(Byte);

typedef struct {
	Byte  SpiId;					// Идентификатор SPI канала (SPIA, SPIB, ...)
	Uns   SpiBaud;					// Частота синхроимпульсов
	unsigned long   Addr;			// Адрес
	Byte  Func;						// Функция
	Uns   Count;					// Количество байт данных
	Uns  *Buffer;					// Указатель на буфер данных Byte заменил на Uns
	Byte  RdData;					// Текущий считанный байт
	Byte  WrData;					// Текущий записанный байт
	Byte  State;					// Текущий шаг выполнения
	Bool  Error;					// Флаг ошибки в работе памяти
	Byte  RetryTimer;				// Таймер повторов перезаписи
	Byte  RetryCount;				// Количество повторов перезаписи
	Uns   BusyTimer;				// Таймер окончания записи
	Uns   BusyTime;					// Максимальное время записи
        Uns   SN;
	TCsFunc CsFunc;					// Функция chip select'а
	//void (*CsFunc)(Byte);			// Функция chip select'а
} TFM25V10, *pFM25V10;

void FM25V10_Init(pFM25V10);							// Инициализация
void FM25V10_Update(pFM25V10);							//
Byte FM25V10_ReadStatus(pFM25V10);						// Чтение статусного регистра
void FM25V10_WriteStatus(pFM25V10, Byte Status);		// Запись статусного регистра
Byte FM25V10_ReadByte(pFM25V10, unsigned long Addr);				// Чтение байта
void FM25V10_WriteByte(pFM25V10, unsigned long Addr, Byte Data);	// Запись байта
void FM25V10_WriteEnable(pFM25V10);					// Разрешение записи
void FM25V10_WriteDisable(pFM25V10);					// Запрещение записи

Byte FM25V10_ReadSN(pFM25V10);						// Чтение статусного регистра

extern TFM25V10 		Eeprom1;		// параметры и часть информационного блока
extern TFM25V10			Eeprom2;		// часть информационного блока

#endif
