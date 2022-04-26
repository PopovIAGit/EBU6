
#ifndef INVCONTROL_
#define INVCONTROL_

#include "main.h"
#include "peref_ApFilter1.h"
#include "peref_ApFilter3.h"
/*#include "dmc\aci.h"
#include "dmc\clarke.h"
#include "dmc\ipark.h"
#include "dmc\park.h"
#include "dmc\pid_reg3.h"
#include "dmc\qgen.h"
#include "dmc\rampgen.h"
#include "dmc\S_curve.h"
#include "dmc\S_curve2.h"
#include "dmc\svgen_dq.h"
#include "dmc\svgen_dq_3ph.h"
#include "dmc\jump_dq.h"
#include "dmc\volt_calc.h"
#include "dmc\Autotuning.h"
#include "dmc\IdentRotorPosition.h"
#include "dmc\SLSCos.h"
#include "interp2D.h"*/

#ifdef __cplusplus
extern "C" {
#endif

// Состояния работы
typedef enum {
	iwmLock        = 0x0001,		// блокировка
	iwmReady       = 0x0002,		// готовность
	iwmHeat        = 0x0004,		// подогрев
	iwmRamp        = 0x0008,		// разгон
	iwmMove        = 0x0010,		// движение
	iwmSlowDn      = 0x0020,		// останов
	iwmCoastStop   = 0x0040,		// останов выбегом
	iwmDcBrake     = 0x0080,		// торможение пост. током
	iwmStartTest   = 0x0100,		// предстартовый тест
	iwmSlowDnRev   = 0x0200, 		// останов с реверсом
	iwmAutoTuning  = 0x0400,		// автонастройка
	iwmIdRotorPos  = 0x0800,		// идентификация положения ротора
	iwmAutoTuneSM  = 0x1000			// автонастройка для синхронного двигателя
} TInvWorkMode;

// Команды управления
typedef enum {
	icwNone        = 0x0000,		// нет команды
	icwLock        = 0x0001,		// заблокироваться
	icwUnLock      = 0x0002,		// разблокироваться
	icwHeatOn      = 0x0004,		// подогрев включён
	icwRunForward  = 0x0008,		// вперёд
	icwRunReverse  = 0x0010,		// назад
	icwJogForward  = 0x0020,		// движение в JOG-режиме вперёд
	icwJogReverse  = 0x0040,		// движение в JOG-режиме назад
	icwSlowDown    = 0x0080,		// медленный останов
	icwCoastDown   = 0x0100,		// останов выбегом
	icwDcBrake     = 0x0200,		// торможение пост. током
	icwAutoTuning  = 0x0400,		// автонастройка
	icwIdRotorPos  = 0x0800,		// идентификация положения ротора
	icwAutoTuneSM  = 0x1000			// автонастройка для синхронного двигателя
} TInvCtrlWord;

// Способы управления
typedef enum {
	icmScalar          = 0x0001,	// скалярное управление
	icmScalarCurr      = 0x0002,	// скалярное управление + токоограничение
	icmCurr            = 0x0004,	// токовое управление
	icmVectFdb         = 0x0008,	// векторное с ОС
	icmVectSls         = 0x0010,	// векторное без ОС
	icmVectSMFdb	   = 0x0020,	// векторное с ОС для СМ
	//icmSlsCurSatSmooth = 0x0020,	// скалярно-векторное c ограничением тока  и плаынм наростанием тока
	icmCurr3Relay      = 0x0040,	// токовое управление по 3-ём фазам релейное
	icmScalarFdb       = 0x0080,	// скалярное управление с ОС
	icmScalarCurrFdb   = 0x0100,	// скалярное управление с ОС + токоограничение
	icmSyncCurr        = 0x0200,	// моментный для синхронного двигателя
	icmSyncCurrFdb     = 0x0400,	// режим для синхронного двигателя c ОС
	icmScalCurrComb    = 0x0800,	// скалярное управление + токовое в разгонах и торможении
	icmVectSlsSmooth   = 0x1000,	// векторное без ОС c переходами
	icmVectSlsCurSat   = 0x2000,	// скалярно-векторное c ограничением тока
	icmCurr3           = 0x4000		// токовое управление по 3-ём фазам
} TInvCtrlMethod;

// Состояние сигнальных светодиодов
typedef enum {
	ilsOff   = 0,						// Светодиод выключен
	ilsOn    = 1,						// Светодиод включен
	ilsQuick = 2,						// Светодиод быстро моргает
	ilsSlow  = 3						// Светодиод медленно моргает
} TInvLedsState;

// Направление движения
typedef enum {
  idNone    =  0,          		// Стоп
  idReverse = -1,            	// Назад
  idForward =  1              	// Вперед
} TInvDirection;

// Источник сигнала задания на регулятор
typedef enum {
	rrsDef=0,							// Задание по умолчанию
	rrsDir,								// Прямое задание
	rrsGen								// Задание c генератора
} TRegRefSrc;

// Тип торможения постоянным током
typedef enum {
	dbtOff=0,							// Выключен
	dbtVolt,								// По напряжению
	dbtCurr								// По току
} TDcBrakeType;

// Способ наложения механического тормоза
typedef enum {
	mbsOff=0,							// Выключен
	mbsFreq,								// По частоте
	mbsTime								// По времени
} TMechBrakeSet;

// Структура для управления
typedef struct {
	TInvCtrlWord	Cmd;				// Команда управления
	TInvCtrlMethod Method;			// Способ управления
	Bool           RealDrive;		// Флаг работы с реальным двигателем
	Bool				SpeedEstFlag;	// Флаг разрешения восстановления скорости
	Bool				ForceFlag;		// Флаг форсировки
	Bool				SlowFlag;		// Флаг дотягивания
	Bool				GenFltrFlag;	// Флаг фильтрации выходного сигнала генератора
	LgInt          Ref1;				// Задание 1 на систему управления
	LgInt          PsiRefGen;		// Задание 2 на систему управления
	LgInt          Curr1;			// Значение мгновенного тока в фазе 1
	LgInt          Curr2;			// Значение мгновенного тока в фазе 2
	LgInt				RotSpeed;		// Значение текущей частоты вращения ротора по энкодеру
	Uns				PreMagTime;		// Время начального намагничивания
	Uns				SpdPreScale;	//	Квантование расчёта контура скорости (отн-но INV_CTRL_FREQ)
	Uns				PsiPreScale;	//	Квантование расчёта контура потокосцепления (отн-но INV_CTRL_FREQ)
	LgInt				RR_corr;			// Коррекция сопротивления ротора
	LgInt          PsiRefMot;		// Задание контура потокосцепления для двигательного режима
	LgInt          PsiRefForce;	// Задание контура потокосцепления для малой скорости
  LgInt          LoadTorqGain;	// Корректировка для расчёта момента нагрузки на валу ЭД
	Uns	 			PwmType;			// Тип используемой ШИМ
	Uns				PsiRType;		// Способ форм. магн.состояния при ВУ с ОС
	LgInt				MaxCurrLevel;	// Максимально-допустимый уровень для токоограничения
	LgInt				RefGain;			// Масштабный коэффициент задания
	LgInt				HallAngle;		// Угол вектора положения ротора от датчиков Холла
	LgInt				DeltaUdc;		// Относительное изменение Udc
	LgInt				UdcMash;			// Масштабный коэффициент для расчета выходного напряжения
	Uns				Udc;				// Уровень Udc
	Uns				UdcComp;			// Компенсация изменений Udc
	Uns				GenType;			// Тип генератора сигналов
	TRegRefSrc     CurrRefSrc;		// Выбор источника задания на регулятор тока
	TRegRefSrc     SpdRefSrc;		// Выбор источника задания на регулятор скорости
	TRegRefSrc		PsiRefSrc;		// Выбор источника задания на регулятор потокосцепления
	Uns				CurrConn;		// Режим подключения токов
	Bool				CurrMode;		// Флаг токового управления для режима Векторного без ОС
	Bool				CurrModeAngle; // Вспомогательный флаг для выравнивания пилы
	Bool				CurrModeSt;		// Флаг токового управления для режима Векторного без ОС при останове
	Bool				CurrModeAngleSt;// Вспомогательный флаг для выравнивания пилы  при останове
	LgInt				CurrSwFreqLev;	// Частота переключения токового управления в режиме Векторного без ОС
	LgInt				CurrLoadCoef;	// Коэффициент расчёта момента при токовом управлении
	LgUns				Curr_Ref_Stop;		// Скалярно-векторное управление
	LgInt				Curr_Kp_Stop;
	LgInt				Curr_Ki_Stop;
	LgInt				Curr_Kc_Stop;
	LgInt				Bound;
	LgInt				Bound_Start;
	LgInt				Bound_Stop;
	LgUns				Curr_Ref_Init;		// Нижнее ограничение тока при стопе (было Начальное задание тока)
	LgUns				Ti_Curr;				// Интенсивность спадания тока при стопе (было Интенсивность наростания тока)
	LgUns				Curr_Ref_Smooth;	// Плавное спадание тока
	LgUns				K_Curr_Stop;		// Коэфф. умен. задания тока при торм.
	Uns				Move_flag;			// Флаг начала движения привода
	Uns				Move_Init_flag; 	// Флаг присвоения начального значения тока при разгоне
	Uns				Stop_flag;			// Флаг останова
	TMechBrakeSet	MechBrakeSet;
	LgInt			MechBrakeSetFreq;
	Uns				LiftObject;			// Режим управления лифтом
	LgInt			EstLoadReset;		// Переменная для сброса нагрузки
	Uns				LiftRampDn;			// Статус замедления в лифтовом режиме
	LgInt			LiftPreDefLoad;		// Предварительная нагрузка определённая при движении кабины	
	LgInt			LiftDefLoad;		// Нагрузка определённая при движении кабины	
	Uns				LiftRampDnFlag;		// Флаг расчёта нагрузки
	LgInt			LiftRampLoadGain;	// Коррекция U/f при останове
	LgInt			LiftRampFreqGain;	// Коррекция дельта-f при дотягивании
	LgInt			LiftFreqAdd;		// Добавка по частоте
	Uns				LiftModeTimer;		// Таймер работы в лифтовом режиме
	LgInt			LiftVoltAdd0;		// Добавка 1 по U/f
	LgInt			LiftVoltAdd1;		// Добавка 2 по U/f
	LgInt			LiftVoltAdd2;		// Добавка 3 по U/f
	Uns				ChangeVoltFlag;		// Флаг возвращения к исходным параметрам U/f
	Uns				OneFloorFlag;		// Флаг поэтажного разъезда
} TInvCtrl;

// Структура статусной информации
typedef struct {
	TInvWorkMode   WorkMode;		// Текущий режим работы
	Bool				InvEnable;		// Флаг работы инвертора
	Bool				JogFlag;			// Флаг работы в JOG-режиме
	TInvDirection	Direction;		// Направление движения
	LgInt				FreqOut;			// Выходная частота в о.е.
	LgInt				VoltOut;			// Выходное напряжение в о.е.
	LgInt				RefOut;			// Выход задатчика интенсивности
	LgInt				RampOut;			// Выход пилы ориентации вектора напряжения
	TInvLedsState	LedForw;			// Управление светодиодом "Вперед"
	TInvLedsState	LedRev;			// Управление светодиодом "Назад"
	Uns				WishRotCmd;    // Желаемое направление вращения при местном управлении
	Uns				CmdPrev;			// Предыдущая команда на движение
	Uns				RefSign;			// Знак задания
	LgInt				RefSpeed;		// Задание для частоты вращения ротора
	LgInt				EstSpeed;		// Восстановленное значение частоты вращения ротора
	LgInt          LoadTorque;		// Момент нагрузки на валу ЭД
	Uns				PreMagTimer;	// Таймер для начального намагничивания
	Uns				SpdScTimer;		// Таймер для расчёта контура скорости
	Uns				PsiScTimer;		// Таймер для расчёта контура потокосцепления
	LgInt				DeltaRefErr;	// Ошибка задания
	LgInt				VoltAngle;		// Угол поворота вектора напряжения	
	LgInt				DeltAngle;		// Разница между векторами напряжения и тока
	LgInt				Ta;				// Сигнал сравнения ШИМ на ключ в фазе А
	LgInt				Tb;				// Сигнал сравнения ШИМ на ключ в фазе В
	LgInt				Tc;				// Сигнал сравнения ШИМ на ключ в фазе С
	Uns				AlarmFlag;		// Флаг выставления аварийной сигнализации
	//INTERP2D		        *vhz;				// Кривая V/F для скалярного управления двигателем
} TInvStatus;

// Структурура конфигурации задатчика интенсивности
typedef struct {
	LgInt Target;						// Задание
	LgInt RelUpTime;					// Относительное время разгона
	LgInt RelDnTime;					// Относительное время останова
	LgInt AbsUpTime;					// Абсолютное время разгона
	LgInt AbsDnTime;					// Абсолютное время останова
	LgInt SlowDnTime1;				// Время замедления на дотягивании 1
	LgInt SlowDnTime2;				// Время замедления на дотягивании 2
} TInvRmpConfig;

// Структура для реализации динамического торможения двигателя
typedef struct {
	TDcBrakeType Type;				// Тип торможения постоянным током
	Bool	Flag;							// Флаг окончания динамического торможения
	Uns	SetTime;						// Продолжительность динамического торможения
	Uns	Timer;						// Таймер динамического торможения
	Uns   Mask;							// Маска режимов для которых работает динамическое торможение
	LgInt	Level;						// Уровень торможения постоянным током
	LgInt	Freq;							// Частота для включения торможения постоянным током
} TInvDcBrake;

// Структура для реализации функции подогрева двигателя
typedef struct {
	Uns   Period;						// Повторяемость включения подогрева
	Uns   OnTime;						// Длительность включения подогрева
	Uns   Timer;						// Таймер подогрева
	Uns   PhaseSeqTime;				// Время переключения фаз
	Uns   PhaseSeqTimer;				// Таймер переключения фаз
	Uns   CurrPhase;					// Текущяя фаза
	LgInt Reference;					// Уровень подогрева
} TInvDcHeat;

// Структура для плавного снятия напряжения с инвертора
typedef struct {
	LgInt RefD;							// Задание на ось D
	LgInt RefQ;							// Задание на ось Q
	LgInt DeltaPower;					// Шаг снятия тока
	LgInt Gain;							// Коэффициент снижения тока
} TInvCoastStop;

// Структура для работы с предстартовым тестом
// тестом на КЗ и тестом проверки защит по току
typedef struct {
	Bool	Flag;						// Флаг окончания теста
	Uns		Stage;						// Текущяя стадия теста
	Uns		Timer;						// Таймер на текущей стадии
	Uns		TimeCalib;					// Время для калибровки смещениний АЦП токов
	Uns		TimePhase;					// Время проверки каждой фазы тока
	Uns		Enabled_TestSC;				// Флаг теста кз при пуске
	Uns		TestingEnabled;				// Флаг разрешения на проведение тестов защиты и КЗ
	Uns		LenOpenKey;					// Длительность открытия ключа при тесте на КЗ
	Uns		IntervalOpenKey;			// Пауза между открытиями ключей 
	Uns		CountOpenKey;				// Количество тестов на КЗ
	Uns		StepTestSC;					// Шаг проверки ключей на КЗ
	Uns		CountOpenKey_Counter;		// Счетчик колличества управляющих импульсов (тестов)
	Uns		IntervalOpenKey_Counter;	// Счётчик интервала открытия ключей
	Uns		Type;
	LgUns 	Q_ref_integralProtect;
	LgUns 	K_U;
	LgUns 	Dt;
	Uns		PhazeProtect;
	Uns 		TestProtect_FAZA;
	Uns		CurrTest;
	Uns		Test_EnabledKey;
	Uns		Next_FAZA;
	Uns		TestFlagForward;
	Uns		TestRun;
	Uns		Old_FAZA;
	Uns		Test_EnabledKey2;
	Uns		StopFromScalar;
	LgInt		I_U;
	LgInt		I_V;
	LgInt		Result_I_W;
	Uns		Work_TestGroundSC;//флаг разрешающий тест кз на ремлю в штатном режиме
	Uns		ReadyTestSCGround;
	Uns		PredTestDmcMethod;
	Uns		PredTestDriveCommand;
	Uns		PredTestDviveMethod;
	Uns 		Key_Test;
	Uns		My_STOP;
	Uns 		EnabledTimerNextStep;
	Uns		next_stop;
	Uns 		PredStartTest;
	Uns 		Model;
	Uns		State;
	Uns		FlagOnBreak;
	Uns		BrResEnable;	// флаг тестирования тормозного сопротивления
	Uns		FactoryCommonModel; // тип блока 11 или 18 кВт
	Uns 		End_TestSC;	// флаг окончани всех предстартовых тестов (разрешает автотюнинг)
	Uns 		EngineCMD;	// флаг запуска автотюнинга для библиотеки InvertorControl
	Uns 		tmp;
} TInvStartTest;

// Струкутра для компенсации момента
typedef struct {
	LgInt InputVolt;  				// Входное напряжение
	LgInt OutputVolt; 				// Выходное напряжение
	LgInt Referense;				// Направление движения
	LgInt Gain; 	  				// Коэффициент компенсации момента
	LgInt GenGain; 	  				// Коэффициент компенсации момента в ген.режиме
	LgInt RmsCurrent; 				// Действующее значение статорного тока
	LgInt Rs;						// Сопротивление статора
	LgInt OutMax;	  				// Максимальное ограничение выхода
	LgInt VoltAddIR;	  			// Вольтодобавка IR-компенсации
	LgInt VoltAddLoad;	  			// Вольтодобавка по нагрузке
	LgInt LoadTorque;	  			// Момент нагрузки
} TInvTorqComp;

// Структура для компенсации скольжения
typedef struct {
	LgInt InputRef;   				// Входная частота
	LgInt OutputRef;  				// Выходная частота
	LgInt Gain; 	   				// Коэффициент компенсации скольжения
	LgInt Load; 	   				// Момент нагрузки
	LgInt CmprLoad;   				// Пороговое значение момента нагрузки
	LgInt OutMax;	   				// Максимальное ограничение выхода
	LgInt RefAdd;	   				// Коррекция задания
} TInvSleepComp;

// Структура для компенсации скольжения в скалярном режиме
typedef struct {
	LgInt InputRef;   				// Входная частота
	LgInt OutputRef;  				// Выходная частота
	LgInt Load;  				// Нагрузка на валу АД
	LgInt UserGain; 	   			// Коэффициент компенсации скольжения (пользователь)
	LgInt Gain;		 	   			// Коэффициент компенсации скольжения
	LgInt GenGain;		 	   			// Коэффициент компенсации скольжения в генер.режиме
	LgInt Tf;   					// Постоянная времени компенсации скольжения
	LgInt OutMax;	   				// Максимальное ограничение выхода
	LgInt RefAdd;	   				// Коррекция задания по частоте
} TInvSleepCompScal;

typedef struct {
	Uns   Mode;
	LgInt KoefK;
	LgInt KoefB;
	LgInt PrevKHiSpeed;
	LgInt PrevKLowSpeed;
	LgInt PrevLvHiSpeed;
	LgInt PrevLvLowSpeed;
	LgInt DeltaRefLevel;
}
TKspeedAdapt;

typedef struct {
	LgInt RefLevel;
	LgInt FdbLevel;
	LgInt Gain1;
	LgInt Gain2;
	LgInt MaxOut;
	LgInt TmpMaxOut;
	LgInt Rezerv1;
	LgInt Rezerv2;
	LgInt Rezerv3;
}
TPosReg;

// Структура для управелния синхронным двигателем
typedef struct {
	Uns   SmMode;
	Uns	  SmTestMode; 
	LgInt TestVolt;
	LgInt TestFreq;
	LgInt TestAngle;
	LgInt MotorAngle;
	LgInt TestZeroCurr; 
	LgInt MotorSpeed;
	Uns   ScaleAngle;
	LgInt ElectrAngle;
	Uns   LiftState;
	Uns	  BrakeDelayTimer;
	Uns	  BrakeDelayTimeOut;
	LgInt PrevElectrAngle;
	LgInt TestSpeed;
	LgInt LowSpeedLevel;
	LgInt HiSpeedLevel;
	TKspeedAdapt SpedReg;
	TPosReg PosReg;
	Uns StartPosMode;
	LgInt StartCorr;
	LgInt SpdTfBase;
	LgInt SpdTfStart;
	LgInt SpdTfActive;
} TInvSMCntrl;


// Основная сруктура библиотеки
typedef struct _TInvControl {
	TInvCtrl			Control;	// Структура для управления
	TInvStatus			Status;		// Структура статусной информации
	TInvRmpConfig		RefRmp;		// Структура для задатчика интенсивности
	TInvRmpConfig		JogRmp;		// Структура для задатчика интенсивности в JOG-режиме
	TInvDcBrake			DcBrake;		// Структура для реализации динамического торможения двигателя
	TInvDcHeat			DcHeat;		// Структура для реализации функции подогрева двигателя
	TInvCoastStop		CoastStop;	// Структура для плавного снятия напряжения с инвертора
	TInvStartTest     	Test;		// Структура для работы с предстартовым тестом
/*	TAutotuning			Autotuning;	// Автотюнинг для АД
	TAutotuneSM			AutotuneSM;	// Автотюнинг для СД
	TIRIP				IdentRotPos;// Идентификация положения ротора
	TSLSCos				SLSCos;		// Бездатчиковое управление
	SVGENDQ				svgen;		// Генерация пространственно-ориентированной ШИМ
	SVGENDQ_3PH			svgen3ph;	// Генерация синусоидального ШИМ
	PARK				park;		// Прямое преобразование Парка
	IPARK       		ipark;		// Обратное преобразования Парка
	RAMPGEN				rg1;		// Генерация сигнала пилообразной формы
	INTERP2D			vhzUp;		// Разгонная кривая V/F для скалярного управления двигателем
	INTERP2D			vhzDn;		// Остановочная кривая V/F для скалярного управления двигателем
	CLARKE            	clarke;		// Прямое преобразования Кларка
	PIDREG3 			RegIa;		// Регулятор тока фазы A
	PIDREG3 			RegIb;		// Регулятор тока фазы B
	PIDREG3 			RegIc;		// Регулятор тока фазы С
	PIDREG3 			SpdReg;		// Регулятор скорости
	PIDREG3 			PsiReg;		// Регулятор потокосцепления
	PIDREG3 			PosReg;		// Регулятор положения
	PIDREG3				SpdRegNorm;	// Регулятор скорости в обычном режиме
	PIDREG3				SpdRegForce;// Регулятор скорости при форсировке
	PIDREG3 			SpdRegAccel;// Регулятор скорости для ускорений/замедлений
	PIDREG3 			MaxCurrReg;	// Регулятор токоограничения в скалярном управлении
	PIDREG3 			SpdRegStart;// Регулятор для предупреждения отката*/
	APFILTER3	 		F1;			// Фильтр задания потокосцепления	
	APFILTER3	 		F2;			// Фильтр выхода регулятора скорости
	APFILTER3	 		F3;			// Фильтр коэффициента услиления регулятора скорости
	APFILTER3	 		F4;			// Фильтр интегрального коэффициента регулятора скорости
	APFILTER3	 		F5;			// Фильтр выхода задатчика интенсивности
	APFILTER3	 		F6;			// Фильтр выхода компенсации момента
	APFILTER3	 		F7;			// Фильтр выхода компенсации скольжения
	APFILTER1			F8;			// Фильтр на выходе генератора сигналов
 	APFILTER3	 		F9;			// Фильтр системы компенсации скольжения
 	APFILTER3 			FiltrVa;	// Фильтр Va
 	APFILTER3 			FiltrVb;	// Фильтр Vb
	APFILTER3 			FiltrLoad;	// Фильтр для расчёта нагрузки
	APFILTER3	 		FCos;		// Фильтр выхода задатчика интенсивности
	APFILTER3	 		SpdTfFiltr;	// Фильтр изменения постоянной времени в ОС по скорости
	TInvTorqComp		TorqComp;	// Компенсация момента при скалярном управлении
	TInvSleepComp		SleepComp;	// Компенсация скольжения
	TInvSleepCompScal	SleepCompScal;// Компенсация скольжения для скалярного режима
	TInvSMCntrl			SMControl;	// Управление СД
	/*S_CURVE				SCurve;		// Формирователь S-образной характеристики
  	S_CURVE2			SCurve2;	// Формирователь S-образной характеристики
  	INTERP2D			PsiR;		// Формирование адаптивного потокосцепления
	CURMOD            	cm;			// Токовая модель для косвенной ориентации
	JUMP_DQ			  	Jump_d;		// 
	JUMP_DQ			  	Jump_q;		// 
	PHASEVOLTAGE		volt;		// Расчет мгновенного значения напряжения на выходе инвертора
	ACIFE             	fe;			// Модель восстановления потока ротора
	ACISE             	se;			// Модель восстановления скорости
	QGEN              	gen;		// Генератор сигналов*/

	void (*OnPowerKeysCtrl)(Bool State);
	void (*OnUnLockDevice)(void);
	Bool (*OnLockStart)(void);
	void (*OnTestCalib)(Bool State);
} TInvControl;

void InvHiControl(TInvControl *);
void InvLoControl(TInvControl *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif


