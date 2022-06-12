#ifndef _RIM_DEVICES_H_
#define _RIM_DEVICES_H_

#include "main.h"
#include "peref_ApFilter1.h"  		// PIA 03.04.14
#include "peref_ApFilter3.h"  		// PIA 04.04.14
#include "peref_SensObserver.h" 	// PIA 07.04.14
#include "peref_SinObserver.h"		// PIA 08.04.14
#include "peref_Clock.h"
#include "peref_Calibs.h"
#include "CMS58M.h"

#include "peref_display.h"
#include "peref_displhal.h"

// Макросы
#define BTN_TIME			(0.200 * PRD_50HZ)		// Время удержании ручки в положении
#define BTN_LEVEL			1


//==========Функция линейной интерполяции==============================
// Формула:		F(x1) - F(x0)
//  F(x) = F(x0) + --------------- * (x - x0)
//			x1   -   x0
//---------------------------------------------------------------------
#define LinearInterpolation(x1, y1, x2, y2, x) \
					(y1 +  ( (y2 - y1)*(x - x1) )/(x2 - x1) ) 

#define DOTS 33

//--------Структура "точка"--------------------------------------------
typedef struct {
	Uns				proc;		// выходное значение
	Uns				adc;		// Величина АЦП 
} TDot;

//---------------------------------------------------------------------
typedef struct {
	TDot						dots[DOTS];	        // Массив точек для интерполяции
	Uns					        input;		        // Вход: АЦП 
	Uns						output;	                // Выход: интерполированная величина
	Bool						fault;		        // Флаг сбоя 
} TLineObserver;





//--------------------------------------------------------------------------------------



#define ADC_CHANNELS_NUM_1                                         6  // 0 UL1_ADC   1 UL3_ADC   2 I_brake_A   3 UL2_ADC  4 TMP_DV_A  5 TempModule_A
#define ADC_CHANNELS_NUM_2                                         4  // 0 VDC_A     1 IU_ADC    2 IV_ADC    3 IW_ADC


// Структура для работы с драйвером
typedef struct ADT7301 {
	Int  *Temper;  			// Температура в градусах
	Byte  ErrTimer;			// Счетчик сбоев
	Byte  ErrCount;			// Количество сбоев до выставления ошибки
	Bool  Error;   			// Сбой датчика
} ADT7301;
//MCP32S17-----------------------(IOCON.BANK = 0)--------------------
#define MCPS17_AAA      ((uint8_t)0x00)
#define MCPS17_ADDRESS  ((uint8_t)0x40)

#define MCPS17_WRITE    ((uint8_t)0x00)
#define MCPS17_READ     ((uint8_t)0x01)

#define MCPS17_IODIRA   ((uint8_t)0x00)
#define MCPS17_IODIRB   ((uint8_t)0x01)

#define MCPS17_IPOLA    ((uint8_t)0x02)
#define MCPS17_IPOLA    ((uint8_t)0x03)

#define MCPS17_GPINTENA ((uint8_t)0x04)
#define MCPS17_GPINTENB ((uint8_t)0x05)

#define MCPS17_DEFVALA  ((uint8_t)0x06)
#define MCPS17_DEFVALB  ((uint8_t)0x07)

#define MCPS17_INTCONA  ((uint8_t)0x08)
#define MCPS17_INTCONB  ((uint8_t)0x08).

#define MCPS17_IOCONA   ((uint8_t)0x0A)
#define MCPS17_IOCONB   ((uint8_t)0x0B)

#define MCPS17_GPPUA    ((uint8_t)0x0C)
#define MCPS17_GPPUB    ((uint8_t)0x0D)

#define MCPS17_INTFA    ((uint8_t)0x0E)
#define MCPS17_INTFB    ((uint8_t)0x0F)

#define MCPS17_INTCAPA  ((uint8_t)0x10)
#define MCPS17_INTCAPB  ((uint8_t)0x11)

#define MCPS17_GPIOA    ((uint8_t)0x12)
#define MCPS17_GPIOB    ((uint8_t)0x13)

#define MCPS17_OLATA    ((uint8_t)0x14)
#define MCPS17_OLATB    ((uint8_t)0x15)

//-------
// Состояние дискретных выходов ТС
//--------------------------------------------------------------------
// Конфигурация ADS1118------------------------------------------
typedef union _TADS1118 {
	Uns all;
	struct {
		Uns OS:1;		// 0	
		Uns NOP:2;		// 1-2	
		Uns PULL_UP_EN:1;	// 3	
		Uns TS_MODE:1;		// 4	
		Uns DR:3;		// 5-7	
		Uns MODE:1;		// 8	
		Uns PGA:3;		// 9-11	
		Uns MUX:3;		// 12-14	
		Uns CNV_RDY_FL:1;		// 15	
		
	} bit;
} TADS1118;
//-------------------------------------------------------------------

// Типы логик обработки
typedef enum {
	ltDigital = 0,	// Дискретный вход
	ltAnMax,			// Аналоговый по максимуму
	ltAnMin,			// Аналоговый по минимуму
	ltAbs,    		// Аналоговый по по модулю
	ltZone,       	// Аналоговый с мертвой зоной
	ltMaxSign,		// Аналоговый по максимуму знаковый
	ltMinSign		// Аналоговый по минимуму знаковый
} TLogType;

// Структура для уровневой обработки сигнала
typedef struct LOG_INPUT {
	TLogType LogType;		// Тип логики обработки
	Bool     Enable;		// Разрешение работы	
	Uns     *Input;		// Вход
	Uns     *Output;		// Выход
	Uns     *Level;		// Уровень срабатывания
	Uns     *Timeout;		// Тайм-аут срабатывания
	Uns      BitMask;		// Маска бита выхода
	Uns      Scale;		// Коэффициент масштабирования времени
	Uns      Timer;		// Таймер срабатывания
	Bool     Match;		// Режим установки сигнала
	Bool     Flag;			// Состояние установки сигнала
} LOG_INPUT;

typedef struct {

        // Ручки------------------------------------------------------------------------------------
         LOG_INPUT   BtnClose;   
         LOG_INPUT   BtnOpen;
         LOG_INPUT   BtnStop1;
         LOG_INPUT   BtnStop2;
         LOG_INPUT   BtnProg1;
         LOG_INPUT   BtnProg2;
         Uns         BtnStatus; 
         // ADC Sensors ----------------------------------------------------------------------------
         uint16_t adcData1[ADC_CHANNELS_NUM_1];
         uint16_t adcData3[ADC_CHANNELS_NUM_2];
          //--- Фильтры U -----------------
	APFILTER1  			URfltr;
	APFILTER1  			USfltr;
	APFILTER1  			UTfltr;
	APFILTER3  			UR3fltr;
	APFILTER3  			US3fltr;
	APFILTER3  			UT3fltr;
        APFILTER1  			VDCfltr;
	//--- Фильтры I -----------------
	APFILTER1  			IUfltr;
	APFILTER1  			IVfltr;
	APFILTER1  			IWfltr;
	APFILTER3  			IU3fltr;
	APFILTER3  			IV3fltr;
	APFILTER3  			IW3fltr;
        TSensObserver			sensObserver;		// Масштабирование сигналов с датчиков
	TSinObserver			sinObserver;		// Вычисление RMS напряжений
        
        ILEG_FQ                         Ia;
        ILEG_FQ                         Ib;
        ILEG_FQ                         Ic;
	
        APFILTER1 			Phifltr;			// Фильтр угола фи
	APFILTER1 			Umfltr;				// Фильтр среднего напряжения
	APFILTER3 			Imfltr;				// Фильтр среднего тока
        // Дисплей------------------------------------------------------------------------------------
	TDisplay                Display;  		
        // RTC---------------------------------------------------------------------------------------
        RTC_Obj			RtcData;
        RTC_TimeTypeDef         Time;
        RTC_DateTypeDef         Date;
        //--------------------------
        ADT7301                 Temper;
        Int                     BlockTemper;
        Uns                     TenControl;
        // ADC ADS1118------------------------------------
        TADS1118                ADC_Out_Config;
        uint16_t                ADC_Out_data;
        //  MCP23S17-TU---------------------------------------------------
        uint8_t                 TU_data220;
        uint8_t                 TU_data24;
        //  74HC595D TC----------------------------------------------------
        uint8_t                 TS_outData;
        GPIO_PinState           TS_Enable; 
        //  AD5061BRJ DAC------------------------------------------------------
        uint16_t                DAC_data;  
        //-----------------------------------------------------------------
         APFILTER1 		ADCToProcfltr;			// Фильтр угола фи
          
         TLineObserver          ADCtoProc;
         //TDot                   dotsADCtoProc[DOTS];
         TLineObserver          ProctoDAC;
         //TDot                   dotsProctoDAC[DOTS];
         TPerefPosition 	Position;			// Калибровка датчика положения и расчет скорости  
         //энкодер
         sCms58m                 cms58m_1;
        // переменные-----------------------------------------------------------------------------------------
        Bool                    RamUpdFlag;
        Uns                     VoltOn;         // in:   
        Uns                     ModFault;       // In: 
        Uns                     Modul_Off;      // out: 
        Uns                     Mcu220380;      // out:1 220  0 380
        Uns                     Ref15;          // out: 1 3квт   0 1.5квт
      
} TPeref;

// Переопределение для работы с памятью

// Глобальные структуры
extern TPeref g_Peref;

// Прототипы функций
void peref_Init(void);

//прототипы функций
void peref_10HzCalc(TPeref *);
void peref_50HzCalc(TPeref *);
void peref_18KHzCalc(TPeref *);
void peref_2KHzCalc(TPeref *);
void peref_200HzCalc(TPeref *);

void ADT7301_Update(ADT7301 *);
void LogInputCalc(LOG_INPUT *);
void RtcControl(void);

void peref_74HC595D(TPeref *);

void MCP23S17_write(uint8_t addr, uint8_t data);
uint8_t MCP23S17_read(uint8_t addr);
void MCP23S17_init(void);
void MCP23S17_update(TPeref *);

void ADS1118_init(TPeref *p);
void ADS1118_update(TPeref *p);
void ADS_init (void);

void peref_ADCtoPRCObserverInit(TPeref *);
void peref_ProctoDACObserverInit(TPeref *);

void peref_ADCDACtoPRCObserverUpdate(TLineObserver *);

// Работа с Eeprom
void memTest(void);

#endif
