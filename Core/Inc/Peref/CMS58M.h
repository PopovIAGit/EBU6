//******************************************************************************
// Имя файла    :  'CMS58M.h'
// заголовок    :  Драйвер энкодера CMS58M
// Автор        :  Маньянов Р.Р.
// Контакты     :  
// Дата         :  12.10.2018
//******************************************************************************

#ifndef __CMS58M_H
#define __CMS58M_H

#include "std.h"

typedef enum
{
    StartNode           = 0x01,
    StopNode            = 0x02,
    EnterPreoperate     = 0x80,
    ResetNode           = 0x81,
    ResetCommunication  = 0x82,
}eNmtService;

// тип доступа к объектам canopen
typedef enum
{
    ReadOnly    = 0,
    ReadWrite   = 1,
}eAccessType;

// размер типов данных, используется в sObject
typedef enum
{
    None    = 0,
    UInt8   = 1,
    UInt16  = 2,
    Int32   = 4,
    UInt32  = 4,
    String1  = 80
} eSize;

// функции, в адресе Can присутствуют или часть COB-ID can open, адрес can выглядит как функция + nodeId, nodeId - адрес устройства canopen (max 127)
typedef enum
{
    TxNmt       = 0x00,          // управление
    TxSync      = 0x80,         // запрос SYNC
    TxTime      = 0x100,
    RxEmergency = 0x80,    // срочное сообщение об аварии
    
    TxGuard     = 0x700, // сообщения GUARD туда и обратно
    RxGuard     = 0x700,
    
    TxSdo       = 0x600, // SDO запрос к узлу
    RxSdo       = 0x580, // SDO ответ от узла
    
    TxPdo1      = 0x180, // PDO1 от узла
    TxPdo2      = 0x280, // PDO2 от узла
    TxPdo3      = 0x380, // PDO3 от узла
    TxPdo4      = 0x480, // PDO4 от узла
    
    RxPdo1      = 0x200,
    RxPdo2      = 0x300,
    RxPdo3      = 0x400,
    RxPdo4      = 0x500
}eMessageType;

// тип команды SDO, в 0-м байте пакета Can присутствует
typedef enum
{
    ReadRequest     = 0x40,
    ReadResponce    = 0x40,
    WriteRequest    = 0x20,
    WriteResponce   = 0x60,
    Error           = 0x80,
} eCanCommand;

// структура объекта canopen
typedef struct Object
{
    uint16_t    Index;      // индекс
    uint8_t     SubIndex;   // субиндекс
    eSize       Size;       // размер данных в байтах
    eAccessType Access;     // тип доступа
} sObject;

typedef struct
{
    uint8_t     nodeId;   
    uint32_t    value;
    uint32_t    lastLinkTime;
}sCms58m;

void Cms58mInir (sCms58m* cms58m);
void Cms58mStart (sCms58m* cms58m);
void Cms58mConfig (sCms58m* cms58m);
void Cms58mSetValue (uint32_t value, sCms58m* cms58m);
void Cms58mRxHandler (sCms58m* cms58m);

#endif //__CMS58M_H