/*======================================================================
Имя файла:         	volt_calc.c
Автор:             	Саидов В.С.
Версия файла:      	01.01
Дата изменения:		12/02/10
======================================================================*/


#include "volt_calc.h"


void phase_voltage_calc(PHASEVOLTAGE *v)
{	
	if (v->OutOfPhase > 0)
	{
		v->MfuncV1 = -v->MfuncV1;
		v->MfuncV2 = -v->MfuncV2;
		v->MfuncV3 = -v->MfuncV3;     
	}

	// scale the incomming Modulation functions with the DC bus voltage value
	v->MfuncV1 = _IQmpy(v->DcBusVolt, v->MfuncV1);
	v->MfuncV2 = _IQmpy(v->DcBusVolt, v->MfuncV2);    
	v->MfuncV3 = _IQmpy(v->DcBusVolt, v->MfuncV3);

	// calculate the 3 Phase voltages       
	v->VphaseA = _IQmpy((M_2DIV3), v->MfuncV1) - 
	             _IQmpy((M_1DIV3), v->MfuncV2) - 
	             _IQmpy((M_1DIV3), v->MfuncV3);
	v->VphaseB = _IQmpy((M_2DIV3), v->MfuncV2) - 
	             _IQmpy((M_1DIV3), v->MfuncV1) - 
	             _IQmpy((M_1DIV3), v->MfuncV3);
	v->VphaseC = _IQmpy((M_2DIV3), v->MfuncV3) - 
	             _IQmpy((M_1DIV3), v->MfuncV1) - 
	             _IQmpy((M_1DIV3), v->MfuncV2);
	
	// Voltage transformation (a,b,c)  ->  (Alpha,Beta)
	v->Valpha = v->VphaseA;
	v->Vbeta  = _IQmpy(v->VphaseA + (v->VphaseB << 1), (M_1_SQRT3));
}



