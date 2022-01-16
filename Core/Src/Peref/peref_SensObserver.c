/*======================================================================
Имя файла:          peref_SensObserver.c
Автор:				Попов И.А.
Версия файла:		1.0
Дата изменения:		04.04.14
Описание: Обработка датчиков тока и напряжения
Вход  - Фильтрованной значение с АЦП в
Выход - Приведенные значения I и U   в
======================================================================*/

#include "peref.h"

void Peref_SensObserverInit(TSensObserver *p)
{
	p->parSensors.p_UR_Mpy		= &g_Ram.FactoryParam.UR_Mpy;
	p->parSensors.p_US_Mpy 		= &g_Ram.FactoryParam.US_Mpy;
	p->parSensors.p_UT_Mpy 		= &g_Ram.FactoryParam.UT_Mpy;
	p->parSensors.p_UR_Offset 	= &g_Ram.FactoryParam.UR_Offset;
	p->parSensors.p_US_Offset 	= &g_Ram.FactoryParam.US_Offset;
	p->parSensors.p_UT_Offset 	= &g_Ram.FactoryParam.UT_Offset;
	p->parSensors.p_IU_Mpy 		= &g_Ram.FactoryParam.IU_Mpy;
	p->parSensors.p_IV_Mpy 		= &g_Ram.FactoryParam.IV_Mpy;
	p->parSensors.p_IW_Mpy 		= &g_Ram.FactoryParam.IW_Mpy;
	p->parSensors.p_IU_Offset 	= &g_Ram.FactoryParam.IU_Offset;
	p->parSensors.p_IV_Offset 	= &g_Ram.FactoryParam.IV_Offset;
	p->parSensors.p_IW_Offset	= &g_Ram.FactoryParam.IW_Offset;
        
        p->parSensors.p_VDC_AU_Mpy	= &g_Ram.FactoryParam.VDC_AU_Mpy;
        p->parSensors.p_VDC_AU_Offset 	= &g_Ram.FactoryParam.VDC_AU_offset;
        p->parSensors.p_I_brake_A_Mpy	= &g_Ram.FactoryParam.I_brake_A_Mpy;
        p->parSensors.p_I_brake_A_Offset 	= &g_Ram.FactoryParam.I_brake_A_offset;
	p->pOffsetMode = mAutoOffset;
}



void Peref_SensObserverUpdate(TSensObserver *p) // 18000 Гц
{
	p->URinp = g_Peref.adcData1[0];              // 0 UL1_ADC   1 UL3_ADC   2 I_brake_A   3 UL2_ADC  4 TMP_DV_A  5 TempModule_A
	p->USinp = g_Peref.adcData1[3];
	p->UTinp = g_Peref.adcData1[1];
	p->IUinp = g_Peref.adcData3[1];              // 0 VDC_A     1 IU_ADC    2 IV_ADC    3 IW_ADC
	p->IVinp = g_Peref.adcData3[2];
	p->IWinp = g_Peref.adcData3[3];
        
        p->VDC_AUinp = g_Peref.adcData3[0];
        p->I_brake_Ainp = g_Peref.adcData1[2];

	// Преобразование напряжений
	p->URout = ADC_CONV(p->URinp, *p->parSensors.p_UR_Mpy, *p->parSensors.p_UR_Offset);
	p->USout = ADC_CONV(p->USinp, *p->parSensors.p_US_Mpy, *p->parSensors.p_US_Offset);
	p->UTout = ADC_CONV(p->UTinp, *p->parSensors.p_UT_Mpy, *p->parSensors.p_UT_Offset);

	// Преобразование токов
	p->IUout = ADC_CONV(p->IUinp, *p->parSensors.p_IU_Mpy, *p->parSensors.p_IU_Offset);
	p->IVout = ADC_CONV(p->IVinp, *p->parSensors.p_IV_Mpy, *p->parSensors.p_IV_Offset);
	p->IWout = ADC_CONV(p->IWinp, *p->parSensors.p_IW_Mpy, *p->parSensors.p_IW_Offset);
        
        
        p->VDC_AUout    = ADC_CONV(p->VDC_AUinp,        *p->parSensors.p_VDC_AU_Mpy, *p->parSensors.p_VDC_AU_Offset);
        p->I_brake_Aout = ADC_CONV(p->I_brake_Ainp,     *p->parSensors.p_I_brake_A_Mpy, *p->parSensors.p_I_brake_A_Offset);
}




