/*======================================================================
Имя файла:          g_Structs.h
Автор:
Версия файла:
Дата изменения:
Описание:
Описание структур
========================================================================*/

#ifndef G_STRUCTS_
#define G_STRUCTS_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------
//================== СТАТУСЫ ============================
//--------------------------------------------------------
// статусный регистр

//-------------------Статус работы----------------------------
// Статус работы
#define STATUS_MOVE_MASK	0xC

typedef union _TStatusReg {
	Uns all;
	struct {
		
		Uns Stop:1;			// 0     Стоп
		Uns Fault:1;			// 1     Авария
		Uns Closing:1;    		// 2     Идет закрытие
		Uns Opening:1;			// 3     Идет открытие
		Uns Test:1;			// 4     Идет тест
		Uns Closed:1;			// 5     Закрыто
		Uns Opened:1;			// 6     Открыто
		Uns Mufta:1;			// 7     Муфта
		Uns MuDu:1;			// 8     Местное управление
		Uns Ten:1;        		// 9     Включен ТЕН
		Uns EnableBluetooth:1;          // 10    Резервное питание
		Uns Rsvd:1;	                // 11    Промежуточное положение
		Uns CycleMode:1;		// 12	 Резерв
		Uns Program:1;			// 13	 Режим программирования
		Uns Rsvd2:2;		        // 14 -15 	Резерв
	} bit;
} TStatusReg;

// Статус калибровки
typedef enum {
	csNone  = 0,			// Привод не калиброван
	csClose = 1,			// Задано только закрыто
	csOpen  = 2,			// Задано только открыто
	csCalib = 3				// Привод калиброван
} TCalibState;

// Скорость связи ModBus
typedef enum {
	br2400   = 0,			// 2400 Бод
	br4800   = 1,			// 4800 Бод
	br9600   = 2,			// 9600 Бод
	br19200  = 3,			// 19200 Бод
	br38400  = 4,			// 38400 Бод
	br57600  = 5,			// 57600 Бод
	br115200 = 6			// 115200 Бод
} TBaudRate;

// Режим связи
typedef enum {
	pmNone = 0,				// Нет проверки бита четности, 2 стоповых бита
	pmOdd  = 1,				// Проверка на нечетность, 1 стоповый бит
	pmEven = 2				// Проверка на четность, 1 стоповый бит
} TParityMode;

// Структура для работы с ПДУ
typedef struct _TPduData {
	Uns Read;            // Чтение данных
	Uns Write;           // Запись данных
	Uns Rsvd;            // Резерв
	Uns Type;            // Тип меню
	Uns Key;             // Код клавиши
} TPduData;

// Диагностика процесса
#define PROC_NOMOVE_MASK	0x0001
#define PROC_PHENG_MASK		0x0002
#define PROC_CALIB_MASK		0x001C
#define PROC_OVERW_MASK		0x0020
#define PROC_DRV_T_MASK 	0x0040
#define PROC_RESET_MASK		0x00A1
//#define PROCESS_EVLOG_MASK	0x00A1
#define PROCESS_EVLOG_MASK	0x03F3
typedef union _TProcessReg {
	Uns all;
	struct {
		Uns NoMove:1;		// 0     Нет движения
		Uns PhOrd:1;		// 1     Чередование фаз двигателя
		Uns NoClose:1;		// 2     Не задано положение закрыто
		Uns NoOpen:1;	   	// 3     Не задано положение открыто
		Uns NoCalib:1;		// 4     Калибровка не выполнена
		Uns Overway:1;		// 5     Уплотнение не достигнуто
		Uns Mufta:1;		// 6	 Муфта
		Uns MuDuDef:1;		// 7     Ошибка по дискретным входам Му/Ду
		Uns SoftStarter:1;	// 8	 Ошибка УПП
		Uns FireContErr:1;	// 9	 не замкнут конитактор в режиме пожрка
		Uns Rsvd:6;			// 11-15  Резерв
	} bit;
} TProcessReg;

// Диагностика сети
#define NET_UV_MASK			0x0007
#define NET_OV_MASK			0x0038
#define NET_PHO_MASK		0x0040
#define NET_VSK_MASK		0x0080
#define NET_BV_MASK			0x0700
#define NET_OV_MAX_MASK		0x3800
//#define NET_EVLOG_MASK		0x7FFF //0x3F7F
#define NET_EVLOG_MASK		0x3F3F

typedef union _TNetReg {
	Uns all;
	struct {
		Uns UvR:1;			// 0     Понижение напряжения в фазе R
		Uns UvS:1;			// 1     Понижение напряжения в фазе S
		Uns UvT:1;			// 2     Понижение напряжения в фазе T
		Uns OvR:1;			// 3     Превышение напряжения в фазе R
		Uns OvS:1;			// 4     Превышение напряжения в фазе S
		Uns OvT:1;			// 5     Превышение напряжения в фазе T
		Uns Rsvd2:1;		// 6
		Uns VSk:1;			// 7     Асимметрия напряжения входной сети
		Uns BvR:1;			// 8     Обрыв фазы R входной сети
		Uns BvS:1;			// 9     Обрыв фазы S входной сети
		Uns BvT:1;			// 10    Обрыв фазы T входной сети
		Uns OvR_max:1;			// 11    Превышение напряжения в фазе R на 47%
		Uns OvS_max:1;			// 12    Превышение напряжения в фазе S на 47%
		Uns OvT_max:1;			// 13    Превышение напряжения в фазе T на 47%
		Uns Rsvd:2;			// 14-15 Резерв
	} bit;
} TNetReg;

// Диагностика нагрузки
#define LOAD_PHL_MASK		0x0007
#define LOAD_I2T_MASK		0x0008
#define LOAD_SHC_MASK		0x00E0
#define LOAD_UNL_MASK		0x0400
#define LOAD_ISK_MASK		0x0800
#define LOAD_RESET_MASK		0x0807
#define LOAD_EVLOG_MASK		0x00EF
typedef union _TLoadReg {
	Uns all;
	struct {
		Uns PhlU:1;			// 0     Обрыв фазы U
		Uns PhlV:1;			// 1     Обрыв фазы V
		Uns PhlW:1;			// 2     Обрыв фазы W
		Uns I2t:1;			// 3     Время-токовая перегрузка
		Uns Rsvd1:1;			// 4     Резерв
		Uns ShCU:1;			// 5	 Короткое замыкание в фазе U
		Uns ShCV:1;			// 6	 Короткое замыкание в фазе V
		Uns ShCW:1;			// 7	 Короткое замыкание в фазе W
		Uns Rsvd2:3;			// 8-10	 Резерв
		Uns ISkew:1;			// 11    Ассиметрия тока
		Uns Rsvd:4;			// 13-15 Резерв
	} bit;
} TLoadReg;

// Диагностика устройства
#define DEV_ERR_MASK		0x009F
#define DEV_TMP_MASK		0x0060
#define DEV_RSC_MASK		0x0800
//#define DEV_EVLOG_MASK	0x0400
#define DEV_EVLOG_MASK		0x4FFF
typedef union _TDeviceReg {
	Uns all;
  struct {
		Uns PosSens:1;		// 0     Сбой датчика положения
		Uns Memory1:1;		// 1     Сбой памяти 1
		Uns Memory2:1;		// 2     Сбой памяти 2
		Uns Rtc:1;			// 3     Сбой часов реального времени
		Uns TSens:1;		// 4     Сбой датчика температуры
		Uns Th_BCP:1;		// 5     Перегрев блока БКП
		Uns Tl_BCP:1;		// 6     Переохлождение блока БКП
		Uns Th_BCD:1;		// 7     Перегрев блока БКД
		Uns Tl_BCD:1;		// 8     Переохлождение блока БКД
		Uns Dac:1;			// 9     Сбой ЦАП
		Uns LowPower:1;		// 10     Выключение БКД
		Uns NoBCP_Connect:1;// 11   нет связи с БКП
		Uns BatteryLow:1;	// 12   Замена батарейки через 3 года
		Uns BlueNoLaunch:1;	// 13	не запустился БКЭП
		Uns BCP_ErrorType:1;// 14	не верный тип БКП
		Uns Rsvd:1;       	// 15 Резерв
  } bit;
} TDeviceReg;


// Струкутура объединенных аварий
typedef struct {
	TProcessReg Proc;
	TNetReg     Net;
	TLoadReg    Load;
	TDeviceReg  Dev;
} TFltUnion;

// Состояние дискретных входов
typedef union _TInputReg {
  Uns all;
	struct {
		Uns Open:1;		// 0	открыть
		Uns Close:1;		// 1	Закрыть
		Uns Stop:1;		// 2	stop
		Uns DU:1;	        // 5	du
		Uns ResetAlarm:1;       // 3	deblok
		Uns NoUse:11;		// 7-15	Не используются
	} bit;
} TInputReg;

// Состояние дискретных выходов ТС
typedef union _TOutputReg {
	Uns all;
	struct {
		Uns Opened:1;		// 0	Открыто
		Uns Closed:1;		// 1	Закрыто
		Uns Opening:1;		// 2	Открывается
		Uns Closing:1;		// 3	Закрывается
		Uns Fault:1;		// 4	Питание
		Uns MuftaOpen:1;	// 5	Муфта
		Uns MuftaClose:1;	// 6	Авария
		Uns MUDU:1;		// 7	МУ/ДУ
		Uns Rzvd1:1;		// 8	Неисправность
		Uns Rsvd:7;		// 9-15  Резерв
	} bit;
} TOutputReg;


// Группа аварий
typedef enum {
	pgProcess = 0,			// Группа аварий процесса
	pgNet     = 1,			// Группа аварий сети
	pgLoad    = 2,			// Группа аварий нагрузки
	pgDevice  = 3,			// Группа аварий устройства
	pgCount   = 4			// Количество групп
} TPrtGroup;

// Команды БУР
typedef enum
{
	bcmNone = 0,				// Нет команды
	bcmStop,					// Стоп
	bcmClose,					// Закрыть
	bcmOpen,					// Открыть
	bcmSetDefaultsUser,			// Пользовательские параметры по умолчанию
	bcmCalibReset,				// Сброс калибровки
	bcmPrtReset,				// Сброс защит
	bcmLogClear,				// Очистка журнала
	bcmCycleReset,				// Сброс циклов
	bcmSetDefaultsFact,			// Установка заводских параметров по умолчанию
	bcmDefStop,					// Спсевдо команда стоп при авариях, нужна для определения общей продолжительности работы
	bcmDiscrOutTest,			// Тест дискретных выходов
	bcmDiscrInTest,				// Тест дискретных входов
	bcmBlueOn,					// включение Bluetooth
	bcmBlueOff					// выключение Bluetooth
} TBurCmd;

// Тип индикатора
typedef enum {
    it_WINSTAR = 0, // OLED WINSTAR
    it_FUTABA       // VAC FUTABA
} TIndicType;

// Место установки БУР (пожарка - Линейная часть/Автоматика и телемеханика)
typedef enum {
	spLinAuto   = 0,		// 0 Линейная часть/Автоматика и телемеханика
	spFire 		= 1 		// 1 Пожарка
}TSettingPlace;

// Метод останова
typedef enum {
	smSlowDown 		= 0,	// выбег
	smReverse   	= 1,	// реверс
	smDynBreak  	= 2		// динамика
} TStopMethod;

// Управление контакторами д
typedef enum {
	cgStop  = 0,
	cgOpen  = 1,
	cgClose = 2
} TContactorGroup;


// Управление работой защит
typedef enum {
  pmOff       = 0,		// Защита выключена
  pmOn  = 1,		// Сигнализация и останов
  pmCount     = 2		// Количество режимов
} TPrtMode;

// Тип штока
typedef enum {
  rdtNormal  = 0,			// Нормальный
  rdtInverse = 1			// Инверсный
} TRodType;

// Команды задания/сброса
typedef enum {
  trNone     = 0,			// Нет команды
  trTask     = 1,			// Задать
  trReset    = 2 			// Сбросить
} TTaskReset;

// Нормальное состояние входа
typedef enum {
  nsOpened = 0,				// Нормально-открытый контакт
  nsClosed = 1				// Нормально-закрытый контакт
} TNormState;

// Режим индикации тока
typedef enum {
  imRms     = 0,		  	// Действующие значения
  imPercent = 1			  	// В процентах от номинального тока двигателя
} TIndicMode;

// Тип задвижки клин/шибер
typedef enum {
  vtKlin     = 0,		  	// Клиновая задвижка
  vtShiber   = 1			// Шиберная задвижка
} TValveType;


// Тип работы тена
typedef enum {
	tmControl 		= 0, 	// Управление по уставкам
	tmTimerControl	= 1, 	// Управление по уставкам с задержкой времени
	tmAlwaysOn 		= 2,	// Всегда включен
	tmHandControl 	= 3		// Ручное управление
}TTenMode;

// Состояние светодиодов блока
typedef union _TLedsReg {	// МОИ
	Uns all;
	struct {
		Uns Opened:1;     	// 0     Открыто/открываеться
		Uns Closed:1;      	// 1     Закрыто/закрываеся
		Uns Fault:1;       	// 2     авария
		Uns MuDu:1;     	// 4     МУДу
		Uns Rsvd1:1;     	// 3     
		Uns Rsvd2:1;		// 5	 
		Uns Rsvd3:1;       	// 6     
		Uns Rsvd4:1;		// 7	 
		Uns Rsvd5:8;      	// 6-15  Резерв
	} bit;
} TLedsReg;

// Тип входного сигнала
typedef enum {
	it24  =  0,		// 0 Тип сигнала 24 В
	it220 = 1 		// 1 Тип сигнала 220 В
}TInputType;


// Маска дискретных входов
typedef union _TUInvert {
	Uns all;
	 struct {
		Uns Open:1;       	// 0     Открыть
		Uns Close:1;      	// 1     Закрыть
		Uns Stop:1;       	// 2     Стоп open
		Uns DU:1;		// 3	 Му
		Uns ResetAlarm:1;	// 4
		Uns Rsvd:11;      	// 5-15  Резерв
	 } bit;
} TUInvert;

// Маска дискретных выходов
typedef union _TSInvert {
	Uns all;
	 struct {
	 	Uns Dout0:1;		// 0 - Открыто
		Uns Dout1:1;		// 1 - Закрыто
		Uns Dout2:1;		// 2 - открываеться
		Uns Dout3:1;		// 3 - закрываеться
		Uns Dout4:1;		// 4 - авария
		Uns Dout5:1;		// 5 - муфта открытия
		Uns Dout6:1;		// 6 - муфта закрытия
		Uns Dout7:1;		// 7 - му/ду
		Uns Rsvd:8;      	// 8-15  Резерв
	 } bit;
} TSInvert;

// Состояние датчиков холла блока
typedef union _THallBlock {
	Uns all;
	struct {
		Uns Open:1;       // 0     Открыть
		Uns Close:1;      // 1     Закрыть
		Uns Stop1:1;     // 2     Стоп1
		Uns Stop2:1;     // 3     Стоп2
		Uns Prog1:1;     // 4     prog1
		Uns Prog2:1;     // 5     prog2
		Uns Rsvd:10;      // 4-15  Резерв
	} bit;
} THallBlock; 

#ifdef __cplusplus
}
#endif // extern "C"

#endif

