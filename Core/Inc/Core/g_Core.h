/*======================================================================

Inputs 
Outputs
========================================================================*/

#ifndef CORE_
#define CORE_


#include "main.h"
#include "core_Protections.h"
#include "core_InverterControl.h"
#include "core_Menu.h"
#include "csl_memory.h"
#include "core_VlvDrvCtrl.h"
#include "core_TorqueObserver.h"
#include "stat_fm25v10.h"
#include "aci.h"
#include "volt_calc.h"

#define CANCEL_TOUT				(2.000 * PRD_10HZ)
#define START_DELAY_TIME		(2.000 * PRD_50HZ)	// Ограничение времени паузы между остановом и след. запуском
// обработка датчиков холла
#define BTN_OPEN			1
#define BTN_CLOSE			2
#define BTN_STOP1			4
#define BTN_STOP2			8
#define BTN_PROG1               	16
#define BTN_PROG2                       32
#define BTN_STOP                        (BTN_STOP1 | BTN_STOP2)

// Константы управления и индикации
#define BASE_FREQ		5000	// Базовая частота (1 - 0.01 Гц)
#define BASE_VOLTAGE		380	// Базовое напряжения
#define BASE_CURRENT		530	// Базовый ток (1 - 0.1 А)
#define BASE_DC_VOLT		534	// Базовое напряжение в звене пост. тока
#define MAX_Tx_OUT	        0.80

#define   _IQsinPU(A)         sinf((A)*6.283185307)
#define   _IQcosPU(A)         cosf((A)*6.283185307)

#define   _IQsat(A, Pos, Neg)  (fmax(((fmin((A),(Pos)))),(Neg)))
#define   _IQmpy(A,B)         ((A) * (B))
#define   _IQdiv(A,B)        ((float)(A) / (float)(B))

#define LENGTH_TRQ		72	// Размер буфера коэффициентов расчета момента

#define IS_VECTOR               1 // если векторное управление

//------------------- Глобальные переменные --------------------------------


typedef struct {
	Float Alpha;	// Выход: Переменная оси D неподвижной двухфазной системы координат
	Float Beta;		// Выход: Переменная оси Q неподвижной двухфазной системы координат
	Float Angle;	// Вход:  Угол поворота
	Float Ds;		// Вход:  Переменная оси D вращающейся двухфазной системы координат
	Float Qs;		// Вход:  Переменная оси Q вращающейся двухфазной системы координат
} IPARK;	            

typedef struct {
	Float Alpha;	// Вход:  Переменная оси D неподвижной двухфазной системы координат
	Float Beta;		// Вход:  Переменная оси Q неподвижной двухфазной системы координат
	Float Angle;	// Вход:  Угол поворота
	Float Ds;		// Выход: Переменная оси D вращающейся двухфазной системы координат
	Float Qs;		// Выход: Переменная оси Q вращающейся двухфазной системы координат
} PARK;

typedef struct {
	Float Input;
	Float Output;
} Pt2D;

typedef struct {
	Uns   NumPoints;	// Вход:      Количество точек
	Float Input;		// Вход:      Входной сигнал
	Float Output;		// Выход:     Выходной сигнал
	Pt2D  Points[5];	// Параметры: Точки кривой
} INTERP2D;

typedef struct {
	Uns   Period;			// Параметр: Количество тактов на половине периода ШИМа (Q0)
	Float MfuncC1;			// Вход:     Задание ШИМ на 1 и 2 стойку (GLOBAL_Q)
	Float MfuncC2;			// Вход:     Задание ШИМ на 3 и 4 стойку (GLOBAL_Q)
	Float MfuncC3;			// Вход:     Задание ШИМ на 5 и 6 стойку (GLOBAL_Q)
	Uns   Cmpr1;			// Выход:    Регистр сравнения стойки 1 и 2 (Q0)
	Uns   Cmpr2;			// Выход:    Регистр сравнения стойки 3 и 4 (Q0)
	Uns   Cmpr3;			// Выход:    Регистр сравнения стойки 5 и 6 (Q0)
	Uns   Cmpr;				// Выход:    Регистр сравнения для запуска АЦП (Q0)
} PWM;

typedef struct {
	Float Freq;			// Вход:       Частота сигнала пилообразной формы (о.е.)
	Float StepAngleMax;	        // Параметр:   Коэффициент масштабирования шага угла (о.е.)
	Float Angle;			// Переменная: Шаг угла (о.е.)
	Float Gain;			// Вход:       Коэффициент усиления сигнала (о.е.)
	Float Offset;			// Вход:       Коэффициент смещения сигнала (о.е.)
	Float Out;			// Выход:      Сигнал пилообразной формы (о.е.)
} RAMPGEN;

typedef struct {
	Bool  Scalar;		// Вход:  Скалярная система управления
	Float PredmodAmp;	// Вход:  Амплитуда 3-ей гармоники предмодуляции
	Float RampGenOut;	// Вход:  "Пила" основной гармоники
	Float RampGenOut3;      // Параметр: "Пила" 3-ей гармоники для предмодуляции
	Float Ta;		// Выход: Продолжительность включения ключа в фазе А (о.е.)
	Float Tb;		// Выход: Продолжительность включения ключа в фазе B (о.е.)
	Float Tc;		// Выход: Продолжительность включения ключа в фазе C (о.е.)
	Float dVa;		// Вход:  Величина корректирующего воздействия в фазе А (о.е.)
	Float dVb;		// Вход:  Величина корректирующего воздействия в фазе B (о.е.)
	Float dVc;		// Вход:  Величина корректирующего воздействия в фазе C (о.е.)
	Float VaRef;		// Вход/Параметр: Требуемое напряжение в фазе A
	Float VbRef;		// Вход/Параметр: Требуемое напряжение в фазе B
	Float VcRef;		// Вход/Параметр: Требуемое напряжение в фазе C
	Float Valpha;		// Вход/Параметр: Требуемое напряжение в фазе Альфа
	Float Vbeta;		// Вход/Параметр: Требуемое напряжение в фазе Бета
	Float Vds;			// Вход/Параметр: Требуемое напряжение 
	Float Vqs;			// Вход/Параметр: Требуемое напряжение 
	Float	VPredMod;	// Параметр: Напряжение предмодуляции
} SVGENDQ_3PH;

// Режимы работы при цифровом управлении приводом
typedef enum {
	wmStop           = 1,					// Режим стоп
	wmMove     	 = 2,					// Режим движения
	wmDynBreak	 = 3,					// Режим торможения
	wmStart		 = 4,
	wmSpeedTest	 = 5

} TWorkMode;


// Структура для цифрового управления приводом
typedef struct _TDmControl {
	TWorkMode	WorkMode;				// Режим работы
	Int		RequestDir;				// Заданное направление вращения
	LgInt	 	TargetPos;				// Целевое положение
	Uns		TorqueSet;				// Задание момента
	Uns		TorqueSetPr;			// Задание момента в %
	Uns 		BreakFlag;				// Флаг определяющий возможность уплотнения
	Uns 		OverWayFlag;			// Флаг показывающий что уплотнение не достигнуто
	Uns 		MufTimer;				// Таймер срабатывания муфты в движении
	Uns 		CalibStop;				// Остановка по калибровке
	Uns		accelTimer;				// Таймер разгона. Пока он считает, муфта не работает
        Int             RequestPos;
       
} TDmControl;


typedef struct PID_DATA {
  double kp;

  double MAX_OUT;
  double MIN_OUT;

} pData_t;

// Струкутра для компенсации момента
typedef struct {
	Float InputVolt;  				// Входное напряжение
	Float OutputVolt; 				// Выходное напряжение
	Float Referense;				// Направление движения
	Float Gain; 	  				// Коэффициент компенсации момента
	Float GenGain; 	  				// Коэффициент компенсации момента в ген.режиме
	Float RmsCurrent; 				// Действующее значение статорного тока
	Float Rs;						// Сопротивление статора
	Float OutMax;	  				// Максимальное ограничение выхода
	Float VoltAddIR;	  			// Вольтодобавка IR-компенсации
	Float VoltAddLoad;	  			// Вольтодобавка по нагрузке
	Float LoadTorque;	  			// Момент нагрузки
} TInvTorq;

typedef struct {
	
    TStatusReg 			Status;			// Статус работы   
    TCoreProtections	        Protections;	// Защиты
    TCoreVlvDrvCtrl		VlvDrvCtrl;		// Управление задвижкой
    // ---
    TTorqObs			TorqObs;		// Расчет момента
    TInvTorq                    TorqInv;                // расчет момента для инвертора
      //--Работа инвертора---------------------------------------------
    TDmControl                  MotorControl; 
 
    RAMPGEN                     rg1;  
    INTERP2D                    vhz;
    IPARK                       ipark;  
    PARK                        park;
    SVGENDQ_3PH                 svgen3ph;
    PWM                         Pwm;  
    TInvCoastStop               CoastStop;
    TInvDcBrake	                DcBrake;		// Структура для реализации динамического торможения двигателя 
    pData_t                     pidData;
    Float Mash1;			// Масштабный коэффициент для перевода в о.е. (GLOBAL_Q)
    Float Mash2;			// Масштабный коэффициент по времени расчета (GLOBAL_Q)
    Float Mash3;			// Масштабный коэффициент по текущей частоте (Q21)    
    ACISE                       se;
    ACIFE             	        fe;  // Модель восстановления потока ротора
    PHASEVOLTAGE		volt;		// Расчет мгновенного значения напряжения на выходе инвертора     
          
    Uns				NoErrFlag;
    Uns                         DisplayTimer;
    Uns                         DisplayRestartTimer;
    Uns 			DefFlag;
    Uns     			CancelTimer;       // время до отмены
    Uns                         PrevCycle ;
    bool                        DynBreakEnable;
      
} TCore;
//------------------- Протатипы функций ------------------------------------
void Core_Init(TCore *);
void core18kHZupdate(void);
void core200HZupdate(void);
void core50HZupdate(void);
void core10HZupdate(void);

void Core_CalibControl(TCore *);			// Управление калибровкой
void Core_CalibStop(TCore *);				// стоп по калибровке
void Core_DefineCtrlParams(TCore *);		// Задача контролируемых параметров (Моменты - Движения, трогания, уплотнения. Определение текущей зоны и т.д)
void Core_ControlMode(TCore *);				// Стэйт машина
void StopPowerControl(void);					// действия при стопе
void StartPowerControl(TValveCmd ControlWord);                  // Действия при старте


void coreTS(TCore *);
void coreTLocalControl(TCore *);
void coreTU(TCore *);

void rampgen_calc(RAMPGEN *);
void interp2D_calc(INTERP2D *);
void  ipark_calc(IPARK *);
void park_calc(PARK *);
void svgendq3ph_calc(SVGENDQ_3PH *);
void pwm_calc(PWM *);


void Core_Drive_Update(void);
void Drive_ReWrite_Update(void);
double p_Controller(LgInt setPoint, LgInt processValue);

extern TCore g_Core;
#endif // CORE_



