#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "main.h"
#include "std.h"
#include "core_VlvDrvCtrl.h"
#include "core_TorqueObserver.h"
#include "g_Structs.h"
#include "peref_Clock.h"		// Библиотека для преобразований чисел/строк

// ПАРАМЕТРЫ НАСТРОЙКИ
// Группа Status (Адрес = 0, Количество = 40) - Диагностика - просмотр
typedef struct _TStatus
{
	TStatusReg      Status;         // 0. 0 Статус работы
	TFltUnion	Faults;		// 1-4. 1-4 Аварии
	TInputReg       StateTu;     	// 5. 5 Состояние дискретных входов
	TOutputReg      StateTs;	// 6. 6 Состояние дискретных выходов
	Uns             Position;       // 7. 7 Положение
	Uns             Torque;         // 8. 8 Момент
	Int             Speed;		// 9. 9 Скорость
	Uns             Ur;             // 10. 10 Напряжение фазы R
        Uns             Us;             // 11. 11 Напряжение фазы S
        Uns             Ut;             // 12. 12 Напряжение фазы T
	Uns             Iu;             // 13. 13 Ток фазы U
	Uns             Iv;             // 14. 14 Ток фазы V
	Uns             Iw;             // 15. 15 Ток фазы W
	Int             AngleUI;        // 16. 16 Угол нагрузки
	TCalibState     CalibState;     // 17. 17 Статус калибровки
	Int             PositionPr;     // 18. 18 Положение %
	Uns             FullWay;        // 19. 19 Полный ход
	Int             CurWay;         // 20. 20 Текущий ход
	Uns             CycleCnt;       // 21. 21 Счетчик циклов
	Int             Temper;         // 22. 22 Температура блока
	Uns             VersionPO;      // 23. 23 Версия ПО
	Uns 		VDC;	        // 24. 25-39 Резерв
        Uns 		Rsvd25;	        // 25. 25-39 Резерв
        Uns 		Rsvd26;	        // 26. 25-39 Резерв
        Uns 		Rsvd27;	        // 27. 25-39 Резерв
        Uns 		Rsvd28;	        // 28. 25-39 Резерв
        Uns 		Rsvd29;	        // 29. 25-39 Резерв
        Uns 		Rsvd30;	        // 30. 25-39 Резерв
        Uns 		Rsvd31;	        // 31. 25-39 Резерв
        Uns 		Rsvd32;	        // 32. 25-39 Резерв
        Uns 		Rsvd33;	        // 33. 25-39 Резерв
        Uns 		Rsvd34;	        // 34. 25-39 Резерв
        Uns 		Rsvd35;	        // 35. 25-39 Резерв
        Uns 		Rsvd36;	        // 36. 25-39 Резерв
        Uns 		Rsvd37;	        // 37. 25-39 Резерв
        Uns 		Rsvd38;	        // 38. 25-39 Резерв
        Uns 		Rsvd39;	        // 39. 25-39 Резерв
}TStatus;

typedef struct _TUserParam
{      
        Uns		MoveCloseTorque;	// B0. 40.Момент закрытия
	Uns		MoveOpenTorque;		// B1. 41 Момент открытия
	Uns		BreakCloseTorque;	// B2. 42 Момент уплотнения на закрытие
	Uns		StartCloseTorque; 	// B3. 43 Момент трогания на закрытие
	Uns		BreakOpenTorque;  	// B4. 44 Момент уплотнения на открытие
	Uns		StartOpenTorque;  	// B5. 45 Момент трогания на открытие
	Uns		reserv46;		// B6. 46 Резерв
	Uns             BreakMode;		// B7. 47 Тип уплотнения
	Uns             RodType;        	// B8. 48 Тип штока
	Uns		CloseZone;		// B9. 49 Зона закрыто
	Uns		OpenZone;		// B10. 50 Зона открыто
	Uns             PositionAcc;            // B11. 51 Зона смещения
	Uns            	MainCode;        	// B12. 52 Код доступа
	TTimeVar 	DevTime;		// B13. 53 Время
	TDateVar 	DevDate;		// B14. 54 Дата
	Uns		reserv55;		// B15. 55 Корректеровка времени
	Uns		reserv56;		// B16. 56 Резерв
	Uns  	        MuDuSetup;              // B17. 57 Настройка режима МУ/ДУ
	Uns	        DuSource;		// B18. 58 Источник команд ДУ
	Uns         	TuLockSeal;             // B19. 59 Блокировка залипани
	Uns         	TuTime;                 // B20. 60 Время команды
	TInputType 	InputType;		// B21. 61 Тип входного сигнала 24/220
	TUInvert	TuInvert;		// B22. 62 Маска дискретных входов
	TSInvert 	TsInvert;		// B23. 63 Маска дискретных выходов
	TBaudRate       RsBaudRate;             // B24. 64 Скорость связи
	Uns             RsStation;              // B25. 65 Адрес станции
	TParityMode	RsMode;			// B26. 66 Режим связи
	Uns		MuffTimer;		// B27. 67
	Uns		reserv68;		// B28. 68 Резерв
	Uns		reserv69;		// B29. 69 Резерв
        Uns		reserv70;		// B30. 70 Резерв
	Uns		reserv71;		// B31. 71 Резерв
	Uns		NoMoveTime;		// B32. 72 Время отсутствия движения
	Uns		OverwayZone;		// B33. 73 Максимальный путь уплотнения
        Uns		reserv74;		// B34. 74 Резерв
	Uns		reserv75;		// B35. 75 Резерв
 	Uns		reserv76;		// B36. 76 Резерв
 	Uns		reserv77;		// B37. 77 Резерв
 	Uns		reserv78;		// B38. 78 Резерв
 	Uns		reserv79;		// B39. 79 Резерв
        Uns		reserv80;		// B40. 80 Резерв
        Uns		reserv81;		// B41. 81 Резерв
        Uns		reserv82;		// B42. 82 Резерв
        Uns		reserv83;		// B43. 83 Резерв
        Uns		reserv84;		// B44. 84 Резерв
        Uns		reserv85;		// B45. 85 Резерв
        Uns		reserv86;		// B46. 86 Резерв
        Uns		reserv87;		// B47. 87 Резерв
        Uns		reserv88;		// B48. 88 Резерв
        Uns		reserv89;		// B49. 89 Резерв
	  
}TUserParam;

typedef struct _TFactoryParam
{
        Uns             FactCode;           	// C0. 90 Код доступа
	Uns             DriveType;          	// C1. 91 Тип привода
	Uns        	ProductYear;        	// C2. 92 Год изготовления блока
  	Uns             FactoryNumber;      	// C3. 93 Заводской номер блока
  	Uns             MaxTorque;          	// C4. 94 Максимальный момент привода
	Uns             Inom;               	// C5. 95 Номинальный ток
  	Uns             GearRatio;          	// C6. 96 Передаточное число редуктора
        Uns 		IndicatorType;          // C7. 97 Передаточное число редуктора
        Uns		DisplResTout;           // C8. 98 Передаточное число редуктора
        Uns             SetDefaults;            // C9. 99 Передаточное число редуктора
        Uns             SubVersionPO;           // C10. 100 Передаточное число редуктора
        THallBlock      HallBlock;              // c11. 101 Состояние датчиков холла блока
        Uns             IU_Mpy;             	// C12. 102 Корректировка тока фазы U
	Uns             IV_Mpy;             	// C13. 103 Корректировка тока фазы V
	Uns             IW_Mpy;             	// C14. 104 Корректировка тока фазы W
	Uns             IU_Offset;          	// C15. 105 Смещение тока фазы U
	Uns             IV_Offset;          	// C16. 106 Смещение тока фазы V
	Uns             IW_Offset;          	// C17. 107 Смещение тока фазы W
	Uns             UR_Mpy;             	// C18. 108 Корректировка напряжения фазы R
	Uns             US_Mpy;             	// C19. 109 Корректировка напряжения фазы S
	Uns             UT_Mpy;             	// C20. 110 Корректировка напряжения фазы T
	Uns             UR_Offset;          	// C21. 111 Смещение напряжения фазы R
	Uns             US_Offset;          	// C22. 112 Смещение напряжения фазы S
	Uns             UT_Offset;          	// C23. 113 Смещение напряжения фазы T
        Uns             VDC_AU_Mpy;             // С24  114
        Uns             VDC_AU_offset;          // С25  115
        Uns             I_brake_A_Mpy;          // С26  116
        Uns             I_brake_A_offset;       // С27  117
        Uns		SinTf;			// C28. 118 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ СИНУСОВ
	Uns		RmsTf;			// C29. 119 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ RMS
        Uns		OnTen;		        // С30. 120 Выключение тэна
	Uns		Ref15;		        // С31. 121 выбор мощности 3квт или 1.5 квт
 	Uns		MCU220380;		// С32. 122 выбор типа питания 220 380
 	Uns		reserv123;		// С33. 123 Резерв
 	Uns		reserv124;		// С34. 124 Резерв
 	Uns		reserv125;		// С35. 125 Резерв
        Uns		reserv126;		// С36. 126 Резерв
        Uns		reserv127;		// С37. 127 Резерв
        Uns		reserv128;		// С38. 128 Резерв
        Uns		reserv129;		// С39. 129 Резерв
        Uns		reserv130;		// С40. 130 Резерв
        Uns		reserv131;		// С41. 131 Резерв
        Uns		reserv132;		// С42. 132 Резерв
        Uns		reserv133;		// С43. 133 Резерв
        Uns		reserv134;		// С44. 134 Резерв
        Uns		reserv135;		// С45. 135 Резерв
        Uns		reserv136;		// С46. 136 Резерв
	Uns		reserv137;		// С47. 137 Резерв
 	Uns		reserv138;		// С48. 138 Резерв
 	Uns		reserv139;		// С49. 139 Резерв
 	Uns		reserv140;		// С50. 140 Резерв
 	Uns		reserv141;		// С51. 141 Резерв
        Uns		reserv142;		// С52. 142 Резерв
        Uns		reserv143;		// С53. 143 Резерв
        Uns		reserv144;		// С54. 144 Резерв
        Uns		reserv145;		// С55. 145 Резерв
        Uns		reserv146;		// С56. 146 Резерв
        Uns		reserv147;		// С57. 147 Резерв
        Uns		reserv148;		// С58. 148 Резерв
        Uns		reserv149;		// С59. 149 Резерв
        Uns		reserv150;		// С60. 150 Резерв
        Uns		reserv151;		// С61. 151 Резерв
        Uns		reserv152;		// С62. 152 Резерв
        Uns		reserv153;		// С63. 153 Резерв
        Uns		reserv154;		// С64. 154 Резерв
        Uns		reserv155;		// С65. 155 Резерв
        Uns		reserv156;		// С66. 156 Резерв
        Uns		reserv157;		// С67. 157 Резерв
        Uns		reserv158;		// С68. 158 Резерв
        Uns		reserv159;		// С69. 159 Резерв

	
}TFactoryParam;

typedef struct _TComands
{
	TTaskReset      TaskClose;              // D0. 260 Задание закрыто
	TTaskReset      TaskOpen;               // D1. 261 Задание открыто
	Uns             RevOpen;                // D2. 262 Обороты на открытие
	Uns             RevClose;               // D3. 263 Обороты на закрытие
	Uns	        reserv264;		// D4  264 резерв
	Uns             CalibReset;             // D5. 265 Сброс калибровки
	TValveCmd       ControlWord;            // D6. 266 Команда управления
	Uns	       	PrtReset;      	        // D7. 267 Сброс защит
	Uns             SetDefaults;            // D8. 268 Задание параметров по умолчанию
	Uns 	        Rsvd1;			// D9. 269 отчистка журнала событий
	Uns             CycleReset;		// D10. 270 Сброс счетчика циклов
	Uns	        RsReset;		// D11. 271 Сброс связи
	Uns 	        Rsvd[8];		// D12-19. 272-279 Резерв
}TComands;

typedef struct _TTestParam
{
        Uns             Mode;                   // G0. 280 Режим теста
	TLedsReg        LedsReg;                // G1. 281 Тест светодиодов блока
	TOutputReg      OutputReg;		// G2. 282 Тест дискретных выходов
	Int	        DacValue;		// G3. 283 Значение ЦАП
	Uns	        DisplShow;		// G4. 284 Старт дисплея 
        Uns	        reserv285;		// D4  285 резерв
        Uns	        reserv286;		// D4  286 резерв
        Uns	        reserv287;		// D4  287 резерв
        Uns	        reserv288;		// D4  288 резерв
        Uns	        reserv289;		// D4  289 резерв
        Uns	        reserv290;		// D4  290 резерв
        Uns	        reserv291;		// D4  291 резерв
        Uns	        reserv292;		// D4  292 резерв
        Uns	        reserv293;		// D4  293 резерв
        Uns	        reserv294;		// D4  294 резерв
        Uns	        reserv295;		// D4  295 резерв
        Uns	        reserv296;		// D4  296 резерв
        Uns	        reserv297;		// D4  297 резерв
        Uns	        reserv298;		// D4  298 резерв
        Uns	        reserv299;		// D4  299 резерв
        Uns	        reserv300;		// D4  300 резерв
        Uns	        reserv301;		// D4  301 резерв
        Uns	        reserv302;		// D4  302 резерв
        Uns	        reserv303;		// D4  303 резерв
        Uns	        reserv304;		// D4  304 резерв
        Uns	        reserv305;		// D4  305 резерв
        Uns	        reserv306;		// D4  306 резерв
        Uns	        reserv307;		// D4  307 резерв
        Uns	        reserv308;		// D4  308 резерв
        Uns	        reserv309;		// D4  309 резерв
}TTestParam;

typedef struct _THideParam
{
        TCalibState     CalibState;                     // H0. 310 Состояние калибровки
	Uns             CalibRsvd;              // H1. 311 Резерв для калибровки
	LgUns           ClosePosition;    	// H2-3. 312-313 Положение закрыто		???
	LgUns           OpenPosition;     	// H4-5. 314-315 Положение открыто		???
	Uns             Password1;              // H6. 316 Основной пароль
	Uns             Password2;              // H7. 317 Заводской пароль
	Uns             ScFaults;               // H8. 318 Аварии КЗ
	Uns		MuffFault;		 // H9. 319 Запись аварии МУФТА
	Uns             CycleCnt;               // H10. 320 Счетчик циклов
	Uns             RERV321;         // H11. 321 запись аварии ТЕМПЕРАТУРА БКП 110
	Uns		CmdButton;		// H12. 322 Команда с ручек управления
	Uns		RERV323;		// H13. 323 тип БКП для выставления соответсвующего типа привода
	TDateVar 	HideDate;		// H14. 324 Дата
	Uns		Rsvd1;			// H15. 325 Резерв
	Uns		TransCurr;		// H16. 326 Ток перехода
	TCubArray	TqCurr;			// H17-36. 327-346 Ток поверхности
	TCubArray	TqAngUI;		// H37-56. 347-366 Углы нагрузки
	TCubArray	TqAngSf;		// H57-88. 367-386 Углы СИФУ
	Uns             TuState;                // H78. 388 Команды ТУ (открыть закрыть стопО СтопЗ)
	Uns             Umid;             	// H82. 392 Среднее напряжение
	Uns             BKP_Temper;             // H85. 395 Температура БКП
	Uns		Imidpr;			// H86 396 Резерв
	Uns             IuPr;                   // H88. 398 Ток фазы U
	Uns             IvPr;                   // H89. 399 Ток фазы V
	Uns             IwPr;                   // H90. 400 Ток фазы W
	Uns             Imid;			// H91. 401 Средний ток
  	LgUns           Position;               // H92-93. 402-403 Положение
	LgUns  		FullStep;        	// H94-95. 404-405 Полный ход
	TReverseType	ReverseType;            // H97. 407 Тип реверса
	//-------------------------------
	Uns		BadTask_2kHz;        // H102. 412 Регистр "плохих" задач прерывания 2 кГц
	Uns		BadTask_200Hz;       // H103. 413 Регистр "плохих" задач прерывания 200 Гц
	Uns		BadTask_50Hz[3];     // H104-106. 414-416 Регистр "плохих" задач прерывания 50 Гц
	Uns		BadTask_10Hz;        // H107. 417 Регистр "плохих" задач прерывания 10 Гц
	Uns		BadTask_Reset;       // H108. 418 Сброс регистров "плохих" задач
	Uns		CpuTime;             // H109. 419 Процессорное время конкретной задачи
	Uns		TaskList;            // H110. 420 Номер списка задач
	Uns		TaskNumber;          // H111. 421 Номер задачи в списке
	//-------------------------------
	Uns             BusyValue;       	 // 116.Процент исполнения
	Uns      	Rsvd2[3];		 	 // H113-115. 422-425 Резерв
	Uns             StartIndic;			 // H116. 426 Индикация в старте
	Uns 		LogEvAddr;			 // H117. 427 Текущий адрес журнала событий
	Uns 		LogCmdAddr;			 // H118. 428 Текущий адрес журнала команд
	Uns 		LogParamAddr;		 // H119. 429 Текущий адрес журнала изменения параметров
	Uns 		LogSimAddr;		     // H120. 430 Текущий адрес журнала SIM
	Uns		LogEvCount;			 // H121. 431 Количество записанных ячеек журнала событий
	Uns		LogCmdCount;		 // H122. 432 Количество записанных ячеек журнала команд
	Uns		LogParamCount;		 // H123. 433 Количество записанных ячеек журнала изменения параметров
	Uns		LogSimCount;		 // H124. 434 Количество записанных ячеек журнала SIM
	Uns		Seconds;			 // H125. 435 Секунды
	TContactorGroup ContGroup;			 // H126. 436 Управление контакторами
	TBurCmd 	LogControlWord;		 // H127. 437 Команды БУР
	Uns		LogReset;			 // H128. 438 Сброс журналов
	TOutputReg      HideStateTs;	        // 6. 6 Состояние дискретных выходов
        Uns 		Rsvd9[60];			 // H130-189. 440-499 Резерв
  
}THideParam;

typedef struct _TLogEvBuffer
{
	TTimeVar       LogTime;		    // T.Время
	TDateVar       LogDate;		    // D.Дата
	TStatusReg     LogStatus;           // 0.Статус работы
	TFltUnion      LogFaults;	    // 1-4.Аварии
	Int            LogPositionPr;       // 5.Положение %
	Uns            LogTorque;           // 6.Момент
	Uns            LogUr;               // 7.Напряжение фазы R
  	Uns            LogUs;               // 8.Напряжение фазы S
  	Uns            LogUt;               // 9.Напряжение фазы T
	Uns            LogIu;               // 10.Ток фазы U
	Uns            LogIv;               // 11.Ток фазы V
	Uns            LogIw;               // 12.Ток фазы W
	Int            LogTemper;           // 13.Температура блока
	TInputReg      LogStateTu;           // 14.Состояние дискретных входов
	TOutputReg     LogSataeTs;          // 15.Состояние дискретных выходов
	Uns 	       Rsvd[20];			// 16-29.Резерв
} TLog;

typedef struct _TRamLogBuff
{
	TStatusReg     	LogStatus;          // 0.Статус работы
	Int            	LogPositionPr;	    // 1.Положение %
	Uns            	LogTorque;          // 2.Момент
	Uns            	LogUr;              // 3.Напряжение фазы R
  	Uns            	LogUs;              // 4.Напряжение фазы S
  	Uns            	LogUt;              // 5.Напряжение фазы T
	Uns            	LogIu;              // 6.Ток фазы U
	Uns            	LogIv;              // 7.Ток фазы V
	Uns            	LogIw;              // 8.Ток фазы W
	Int            	LogTemper;          // 9.Температура блока
	Uns      	LogInputs;        	// 10.Состояние дискретных входов
	Uns		LogOutputs;			// 11.Состояние дискретных выходов
} TRamLogBuff;

typedef struct _TRam
{
        TStatus         Status;
        TUserParam      UserParam;
        TFactoryParam   FactoryParam;
        TComands        Comands;
        TTestParam      TestParam;
        THideParam	HideParam;
        TLog            Log;
} TRam;


typedef struct _TTestRam
{
        TStatus         Status;
} TTestRam;

#define BUSY_STR_ADR		0
#define CMD_CANC_ADR		1
#define EXPR_STR_ADDR		4
#define MPU_BLOCKED_ADR		32
#define CONFIRM_ADDR		34

#define NUM_ICONS		5
#define CODE_ICO		0x00
#define CONN_ICO		0x01
#define BT_ICO			0x02
#define CODEON_ICO              0x03
#define CODEOFF_ICO             0x04

#define GetAdr(Elem)		        ((LgUns)&(((TRam *)RAM_ADR)->Elem)>>2)



#define RAM_ADR			        0					
#define RAM_SIZE			SIZE(TRam)					
#define RAM_DATA_SIZE			(RAM_SIZE)	//(RAM_SIZE - SIZE(TLogEvBuffer))//(RAM_SIZE - sizeof(TLogEvBuffer))		

#define REG_CYCLE_CNT			GetAdr(FactoryParam.CycleCnt)
#define REG_CALIB_STATE			GetAdr(FactoryParam.CalibState) 



#define MENU_GROUPS_COUNT		5
#define MENU_EVLOG_GROUP		4

#define LOGS_CNT			3									// ?????????? ???????? ??????????

#define LOG_EV_START_ADDR		12000								// ????????? ????? ??????? ?????? ???????? ?????? ???????
#define LOG_EV_DATA_CNT			20		//sizeof(TGroupE) + 2		// ?????????? ????? ? 1 ?????? ???????? ?????? ???????
#define LOG_EV_CNT			500									// ?????????? ??????? ? ?????? ???????
#define LOG_EV_DATA_CELL		1									// ?????????? ???????? ? ???????? ?????? ???????

#define LOG_EV_BUF_START_ADDR	0									// ????????? ????? ?????? ???????
#define LOG_EV_BUF_DATA_CNT		SIZE(TLogEvBuffer)				// ?????????? ????? ? ????? ?????? ??????
#define LOG_EV_BUF_DATA_CELL	5									// ?????????? ????? ?????? ? ????? ??????

#define LOG_EV_BUF_CELL_COUNT	5									// ?????????? ???????? ? ?????? ??????? ???????

#define PREV_LEV_INDEX		0										//??????? ??? ??????????? ??????????? ?????? ????????
#define PREV_LCMD_INDEX		1
#define PREV_LPAR_INDEX		2


 
#define LOG_EV_RAM_DATA_ADR	         GetAdr(GroupE)						// ????? ?????? ??????? ? Ram
#define LOG_EV_RAM_DATA_LADR	        (LOG_EV_RAM_DATA_ADR * LOG_EV_DATA_CNT)
#define LOG_EV_SIZE			(LOG_EV_CNT * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL)
//!!!!

#define LOG_CMD_START_ADDR		1000								// ????????? ????? ??????? ??????
#define LOG_CMD_DATA_CNT		7									// ?????????? ???????????? ? ?????? ????? (???? 5, ?? ????????? ????????? ?? ? ?????????)
#define LOG_CMD_CNT			1140								// ??????? ??????? ?????? (?????????? ???????)

#define LOG_PARAM_START_ADDR	        9000								// ????????? ????? ???????? ????????? ??????????
#define LOG_PARAM_DATA_CNT		5									// ?????????? ???????????? ????? (???? ????????? ???????: ????, ????? ? ?.?.)
#define LOG_PARAM_CNT			500									// ??????? ??????? ????????? ??????????

#define IM_READ_BUF_SIZE		((LOG_EV_DATA_CNT * LOG_EV_DATA_CELL) + (LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL))

#define REG_TORQUE_ADDR			GetAdr(Status.Torque)
#define REG_START_IND			GetAdr(HideParam.StartIndic)
#define REG_LOG_ADDR			GetAdr(HideParam.LogEvAddr)
#define REG_LOG_TIME			GetAdr(Log.LogTime)
#define REG_LOG_DATE			GetAdr(Log.LogDate)

#define REG_CODE			GetAdr(UserParam.MainCode)
#define REG_FCODE			GetAdr(FactoryParam.FactCode)
#define REG_PASSW1			GetAdr(HideParam.Password1)
#define REG_PASSW2			GetAdr(HideParam.Password2)
#define REG_PRODUCT_YEAR		GetAdr(FactoryParam.ProductYear)
#define REG_FACT_NUM			GetAdr(FactoryParam.FactoryNumber)
#define REG_MAX_TRQE			GetAdr(FactoryParam.MaxTorque)
#define REG_I_NOM			GetAdr(FactoryParam.Inom)
#define REG_GEAR_RATIO			GetAdr(FactoryParam.GearRatio)
#define REG_DRIVE_TYPE			GetAdr(UserParam.DriveType)

#define REG_RS_BAUD_RATE	        GetAdr(UserParam.RsBaudRate)

#define REG_TASK_DATE			GetAdr(UserParam.DevDate)
#define REG_TASK_TIME			GetAdr(UserParam.DevTime)
#define REG_INDICATOR_TYPE	        GetAdr(FactoryParam.IndicatorType)
#define REG_MUDUSETUP			GetAdr(UserParam.MuDuSetup)

#define REG_VER_PO			GetAdr(Status.VersionPO)
#define REG_SUBVER_PO		        GetAdr(FactoryParam.SubVersionPO)

#define REG_CALIB_STATE		        GetAdr(HideParam.CalibState)
#define REG_CALIB_CLOSE		        GetAdr(HideParam.ClosePosition)
#define REG_CALIB_OPEN		        REG_CALIB_CLOSE+2

#define CMD_DEFAULTS_USER		0x0010	// ???????????????? ????????? ?? ?????????
#define CMD_RES_CLB			0x0020	// ????? ?????????? ??????? ?????????
#define CMD_RES_FAULT			0x0040	// ????? ?????
#define CMD_CLR_LOG			0x0080	// ??????? ???????
#define CMD_RES_CYCLE			0x0100	// ????? ???????? ??????
#define CMD_PAR_CHANGE			0x0200	// ????????? ??????????
#define CMD_DEFAULTS_FACT 	        0x0400	// ????????? ????????? ?? ?????????

#define TS_SIGNAL_MASK_M 	        2047		// ????? ??? ????????? ?????? ???????? ?? ?????????? ??????????? ???? ????????
#define TS_SIGNAL_MASK_T 	        255		// 

#define NUM_ICONS			5
#define CODE_ICO			0x00
#define CONN_ICO			0x01
#define BT_ICO				0x02
#define CODEON_ICO                      0x03
#define CODEOFF_ICO                     0x04


void g_Ram_Init(TRam *p);
void g_Ram_Update(TRam *p);
void RefreshParams(Uns);

extern TRam g_Ram;

extern TTestRam testRam;

extern bool EvLogFlag;
extern Uns  EvLogCelNum; 

extern Uns ResetMb;

#endif
