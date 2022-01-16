/*======================================================================
Имя файла:          core_Menu.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      
Описание:
Библиотека для работы с меню под двухстрочный индикатор
======================================================================*/

#ifndef CORE_MENU_
#define CORE_MENU_

#include "main.h"
#include "core_MenuGlobal.h"



#define GROUP_EDIT_PAR		1
#define GROUP_FACT_PAR		2

#define GetDcr(Adr, pDcr)	Core_MenuReadDcr(&/*g_Core.*/menu, pDcr, Adr)

#define IsPassword1()		(g_Ram.HideParam.Password1 != 0)
#define IsPassword2()		(g_Ram.HideParam.Password2 != 0)

#define KEY_NONE       0  // Открыть
#define KEY_OPEN       1  // Открыть
#define KEY_CLOSE      2  // Закрыть
#define KEY_STOP       3  // Стоп
#define KEY_UP         4  // Вверх
#define KEY_LEFT       5  // Влево (при редактировании параметра)
#define KEY_RIGHT      6  // Вправо (при редактировании параметра)
#define KEY_DOWN       7  // Вниз
#define KEY_ESCAPE     8  // Сброс
#define KEY_ENTER      9  // Ввод
#define KEY_MENU       10 // Меню				// Не используется
#define KEY_MU_DU      11 // МУДУ				// Не используется
#define KEY_PROG       12 // Программирование	// Не используется

#define KEY_FORWARD		KEY_CLOSE   // Вперед	// Не используется
#define KEY_REVERSE		KEY_OPEN    // Назад	// Не используется


#define BLINK_HI_TOUT			(0.500 * PRD_10HZ)		// Время отображения при редактировании
#define BLINK_LO_TOUT			(0.300 * PRD_10HZ)		// Время скрытия при редактировании
#define BIT_TOUT				(1.000 * PRD_10HZ)		// Пауза при выводе бита

#define PutAddHiStr(Addr)			DISPL_PutHiStr(&g_Peref.Display, ToPtr(&AddStr[Addr]))
#define PutAddLoStr(Addr)			DISPL_PutLoStr(&g_Peref.Display, ToPtr(&AddStr[Addr]))

#define GetSetParPercent(A)			_IQrmpy(A, _IQ(50.0/RAM_DATA_SIZE))

#define MENU_DEFAULT { \
    MS_START, 0, 0, 0, 1, True, True, False, \
    &g_Ram.ramGroupC.MaxTorque, \
    0, 0, 0, False, 0, BLINK_HI_TOUT, BLINK_LO_TOUT, 0xFF, 0, BIT_TOUT, \
    False, 0, 0, \
    ToPtr(&g_Ram), g_Peref.Display.HiStr, g_Peref.Display.LoStr, \
    REG_START_IND, RAM_ADR, "", "", \
    False, False, MENU_EVLOG_GROUP, REG_LOG_ADDR, REG_LOG_TIME, REG_LOG_DATE, &g_Ram.ramGroupH.LogEvCount, \
    0, 0, MENU_GROUPS_COUNT, \
    0, 0, 0, \
    0, 0, LOG_EV_CNT, \
    0, 0, 0, \
    0, 0, 0, 0, \
    FALSE, FALSE, 0, 0, FALSE, 0, \
    4, {GetAdr(ramGroupB.DevTime),GetAdr(ramGroupB.DevDate),GetAdr(ramGroupB.RodType),GetAdr(ramGroupB.ValveType),0},   \
    2, {GetAdr(ramGroupB.MoveCloseTorque),GetAdr(ramGroupB.MoveOpenTorque),0,0,0},    \
    1, {GetAdr(ramGroupD.TaskClose),0,0,0,0},  \
    1, {GetAdr(ramGroupD.TaskOpen),0,0,0,0},   \
    1, {GetAdr(ramGroupD.RevOpen),0,0,0,0},    \
    1, {GetAdr(ramGroupD.RevClose),0,0,0,0},   \
    1, {GetAdr(ramGroupB.BreakCloseTorque),0,0,0,0},   \
    1, {GetAdr(ramGroupB.StartCloseTorque),0,0,0,0},   \
    1, {GetAdr(ramGroupB.BreakOpenTorque),0,0,0,0},    \
    1, {GetAdr(ramGroupB.StartOpenTorque),0,0,0,0},    \
    1, {0,0,0,0,0}, \
    1, {GetAdr(ramGroupB.MuDuSetup),0,0,0,0},  \
    groups, Null, params, values, addr_params_altistar48, "РЕЗЕРВ", \
    StartDispl, EnableEdit, WriteValue/*, GetExpressText, GetExpressState*/\
}

// Структура меню
typedef struct TCoreMenu {
	Byte   State;        // Режим меню
	Char   Key;          // Управляющая команда
	Byte   EditType;     // Тип режима редактирования
	Byte   Level;        // Текущий уровень меню
	Byte   MaxLevel;     // Уровень вложенности меню (1 или 2)
	Bool   Indication;   // Флаг разрешения индикации
	Bool   Update;       // Флаг обновления данных из структур
	Bool   ShowReserved; // Флаг отображения резервов
	Uns   *MinMaxGain;   // Коэффициент диапазона

	Uns    Value;        // Текущее значение параметра
	Byte   Rate;         // Текущий разряд
	Byte   EditRate;     // Максимальный разряд для редактирования
	Bool   Blink;        // Состояния мигания значения
	Uns    BlinkTimer;   // Таймер для мигания
	Uns    BlinkHiTime;  // Время отображения при редактировании
	Uns    BlinkLoTime;  // Время скрытия при редактировании
	Byte   Bit;          // Выводимый бита числа в виде бегущей строки
	Uns    BitTimer;     // Таймер паузы при выводе бита
	Uns    BitTime;      // Пауза при выводе бита

	Bool   SleepActive;  // Состояние режима
	Uns    SleepTimer;   // Таймер режима
	Uns    SleepTime;    // Тайм-аут режима

	Uns   *Data;         // Указатель на буфер данных
	String HiString;     // Указатель на буфер верхней строчки индикатора
	String LoString;     // Указатель на буфер нижней строчки индикатора
	Uns    StartAddr;    // Адрес параметра при выводе в режиме старта
	Uns    StartOffset;  // Смещение адреса при выводе в режиме старта
	Char   BufValue[6];  // Временный буфер при редактирование в режиме 1
	Char   BufTmp[17];   // Временный буфер для хранения единицы измерения/строкового значения

	Bool   EvLogFlag;    // Флаг чтения записи
	Bool   EvLogSelected;// Флаг выбора записи
	Uns    EvLogGroup;   // Номер группы журнала
	Uns    EvLogAdress;  // Адрес параметра текущей записи в журнал
	Uns    EvLogTime;    // Адрес параметра времени записи журнала
	Uns    EvLogDate;    // Адрес параметра даты записи журнала
	Uns	   *EvLogCount;	 // Количество сделанных записей в журнал

	struct MENU_CRD Group;     // Структура координаты группы
	struct MENU_CRD SubGroup;  // Структура координаты подгруппы
	struct MENU_CRD EvLog;     // Структура координаты записи журнала
	struct MENU_CRD Param;     // Структура координаты параметра
	struct MENU_DCR Dcr;       // Структура описания параметра

	Byte   setDefaultGroupNumber; // Номер группы для установки параметров по-умолчанию 1-B 2-C

	struct MENU_EXPRESS Express;	// Структура экспресс настройки

	const struct MENU_GROUP  		*Groups;   // Указатель на структуры групп
	const struct MENU_GROUP  		*SubGroups;// Указатель на структуры подгрупп
	const struct MENU_PARAM  		*Params;   // Указатель на структуры параметров
	const struct MENU_STRING 		*Values;   // Указатель на структуры строковых значений
	struct MENU_STRING        		Rsvd;     // Резервная строка

	void (*StartDispl)(String Text);
	Bool (*EnableEdit)(Uns PaswwPrt);
	Bool (*WriteValue)(Uns Memory, Uns ParAddr, Uns *Value);
//	void (*GetExpressText)(Uns State);
//	Byte (*GetExpressState)(Byte CurrentState, Uns State);

} TCoreMenu, *pCoreMenu;


void Core_MenuInit(TCoreMenu *);
void Core_MenuUpdate(TCoreMenu *);
void Core_MENU_Display(TCoreMenu *);

Bool UpdateCode(Uns, Uns, Uns, Uns);
void Core_SetDeff(TCoreMenu *);
void Core_MenuReadDcr(TCoreMenu *, struct MENU_DCR *, Uns);

void StartDispl(String Str);

Bool EnableEdit(Uns);
Bool WriteValue(Uns, Uns, Uns *);
void SetDefaultValues(TCoreMenu *, Byte *);
void ShowAddStrings(TCoreMenu *, Uns, Uns);
void SetModBusParams(void);
void SetModBusParams(void);
/*
void GetExpressText(Uns State);
Byte GetExpressState(Byte CurrentState, Uns State);*/
void AddControl(void);

extern void PFUNC_strRead(Char *, Char *);
extern void PFUNC_strWrite(Char *, Char *);
extern  Int PFUNC_wordRead(Int *);
extern void PFUNC_wordWrite(Int *, Int);

extern TCoreMenu menu;

#endif



