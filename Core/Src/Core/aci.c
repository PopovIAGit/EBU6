/*======================================================================
Имя файла:         	aci.c
Автор:             	Digital Control Systems Group Texas Instruments
				   	  	
Версия файла:      	01.04
Дата изменения:		23/11/10
======================================================================*/

#include "aci.h"

TAdParams   aci_ad;
TBaseParams aci_base;

//--------------------------------------------------------------------------------------------
/*
void cur_mod_init(CURMOD *v)
{
	LgInt zb, lm, lr, rr;
	LgInt kr, ar, dt, tmp;

	//zb  = _IQ21div(aci_base.ub, aci_base.ib * 10);
	zb  = _IQ21div(aci_base.ub, aci_base.ib);
	tmp = _IQ21div(aci_base.wb, zb);
	
	lm  = _IQmpy(aci_ad.lm, _IQ21toIQ(tmp));
	lr  = _IQmpy(aci_ad.lr, _IQ21toIQ(tmp));
	rr  = _IQdiv(aci_ad.rr, _IQ21toIQ(zb));

	tmp = _IQdiv(lr, lm);
	kr  = _IQdiv(_IQ(1.0), _IQ(1.0) + tmp);
	ar  = _IQdiv(rr, lm + lr);
	dt  = _IQ21toIQ(_IQmpy(aci_base.ts, aci_base.wb));
	tmp = _IQmpy(kr, rr);

	v->K[0] = _IQ(1.0) - _IQmpy(v->rrc, _IQmpy(dt, ar));
	v->K[1] = _IQmpy(v->rrc, _IQmpy(tmp, dt));
	v->K[2] = _IQmpy(v->rrc, tmp);
	v->K[3] = _IQ21toIQ(_IQmpy(aci_base.ts, aci_base.fb));
}

void cur_mod_calc(CURMOD *v)
{	
	v->ime    = _IQmpy(v->K[0], v->ime) + _IQmpy(v->K[1], v->ide);
	v->w_slip = _IQdiv(_IQmpy(v->K[2], v->iqe), v->ime);

	v->we = v->wr + v->w_slip;
	v->theta = v->theta + _IQmpy(v->K[3], v->we);
	
	if (v->theta > _IQ(1.0))      v->theta = v->theta - _IQ(1.0);
	else if (v->theta < _IQ(0.0)) v->theta = v->theta + _IQ(1.0); 
}

void cur_mod2_calc(CURMOD *v)
{	
	v->ime    = _IQsat(_IQmpy(v->K[0], v->ime) + _IQmpy(v->K[1], v->ide), _IQ(1.2), _IQ(0.1));
	v->w_slip = _IQdiv(_IQmpy(v->K[2], v->iqe), v->ime);

	v->we = v->wr + v->w_slip;
	if (!v->Jump)
	{
		if (v->Flag == 1) v->We_sat = 0;	// Разгон
		if (v->Flag == 2) v->We_sat = v->We_sat - _IQmpy(v->We_sat, _IQmpy(v->Ti_sat_stop, v->dt)); // Торможение
	}
	else // Переход на токовое
	{
		v->We_sat = v->We_sat + _IQmpy(v->Ti_sat, v->dt);
	}
	
	// Адаптивное ограничение
	v->we     = _IQsat(v->we, v->Spd_ref + v->We_sat, v->Spd_ref - v->We_sat);
	v->We_sat = _IQsat(v->We_sat, _IQ(1.0), _IQ(0.0));
	
	v->theta = v->theta + _IQmpy(v->K[3], v->we);
	
	if (v->theta > _IQ(1.0))      v->theta = v->theta - _IQ(1.0);
	else if (v->theta < _IQ(0.0)) v->theta = v->theta + _IQ(1.0); 
}
*/
//--------------------------------------------------------------------------------------------
void aci_fe_init(ACIFE *v)
{	
	Float ls, lr, lm, kr, ar, le, zb, dT, rs, rr, tmp1, tmp2;
	
	//zb  = _IQ21toIQ(_IQ21div(aci_base.ub, aci_base.ib));
	zb  = aci_base.ub / (aci_base.ib * 10.0);
	rs  = aci_ad.rs / zb;
	rr  = aci_ad.rr / zb;
	
	tmp1 = 1.0/ aci_base.wb;
//	v->K[10] = tmp1;

	tmp2 = tmp1* zb;
//	v->K[0] = tmp2;

	ls  = aci_ad.ls/ tmp2;
	lr  = aci_ad.lr/ tmp2;
	lm  = aci_ad.lm/ tmp2;
	
	dT  = (aci_base.ts/ tmp1)*2;

	tmp1 = lr/ lm;
	kr  = (1.0)/(1.0 + tmp1);
	
	//tmp = _IQmpy(kr, kr);
	//re  = rs + _IQmpy(rr, tmp);
	
	le  = ls + (kr* lr);
	ar  = rr/(lm + lr);


	v->K[0] = dT * 0.00333;
	v->K[1] = (kr * (rr * dT));
	v->K[2] = (1.0) - (dT* ar);
	v->K[3] = le;
	v->K[4] = kr;
	v->K[5] = rs;
	v->K[6] = dT;
	v->K[7] = ((1.0)/ kr);
	v->K[8] = (le/ kr);
	v->K[9] = (kr* (10.0));
	v->K[10] = (0.1); 
}

void aci_fe_calc(ACIFE *v)
{	
	Float IDsE, Error, OldEmf;
	Float UCompDsS, UCompQsS;
								
	// Park transformation on the measured stator current
	IDsE  = (v->IQsS * _IQsinPU(v->ThetaFlux)); 
	IDsE += (v->IDsS * _IQcosPU(v->ThetaFlux)); 

	// The current model section (Classical Rotor Flux Vector Control Equation)
	v->FluxDrE = (v->K[1]* IDsE) + (v->K[2] * v->FluxDrE);
	//v->CurrDrE = _IQmpy(v->FluxDrE, v->Kir);
	
	// Inverse park transformation on the rotor flux from the current model
	v->FluxDrS = (v->FluxDrE * _IQcosPU(v->ThetaFlux)); 
	v->FluxQrS = (v->FluxDrE * _IQsinPU(v->ThetaFlux)); 

	// Compute the stator flux based on the rotor flux from current model
	v->FluxDsS = _IQmpy(v->K[3], v->IDsS) + _IQmpy(v->K[4], v->FluxDrS);	
	v->FluxQsS = _IQmpy(v->K[3], v->IQsS) + _IQmpy(v->K[4], v->FluxQrS);

	// Conventional PI controller section
	Error    =  v->PsiDsS - v->FluxDsS;
	UCompDsS = _IQmpy(v->K[10], Error) + v->UiDsS;
	v->UiDsS = _IQmpy(v->K[10], _IQmpy(v->K[0], Error)) + v->UiDsS;

	Error    =  v->PsiQsS - v->FluxQsS;
	UCompQsS = _IQmpy(v->K[10], Error) + v->UiQsS;
	v->UiQsS = _IQmpy(v->K[10], _IQmpy(v->K[0], Error)) + v->UiQsS;    

	// Compute the estimated stator flux based on the integral of back emf		
	OldEmf = v->EmfDsS;
	v->EmfDsS = v->UDsS - UCompDsS - _IQmpy(v->K[5], v->IDsS);
	v->PsiDsS = v->PsiDsS + _IQmpy((0.5), _IQmpy(v->K[6], (v->EmfDsS + OldEmf))); 

	OldEmf = v->EmfQsS;
	v->EmfQsS = v->UQsS - UCompQsS - _IQmpy(v->K[5], v->IQsS);
	v->PsiQsS = v->PsiQsS + _IQmpy((0.5), _IQmpy(v->K[6], (v->EmfQsS + OldEmf))); 

	// Compute the estimated rotor flux based on the stator flux from the integral of back emf
	v->PsiDrS = _IQmpy(v->K[7], v->PsiDsS) - _IQmpy(v->K[8], v->IDsS);
	v->PsiQrS = _IQmpy(v->K[7], v->PsiQsS) - _IQmpy(v->K[8], v->IQsS);

	v->Te_est = _IQmpy(v->K[9],(_IQmpy(v->IQsS,v->PsiDrS) - _IQmpy(v->IDsS,v->PsiQrS)));
	
	if (_IQabs(v->Te_est) < v->ZeroLoadEst) v->Te_est = 0;

	// Compute the rotor flux angle
	v->ThetaFlux = _IQatan2PU(v->PsiQrS,v->PsiDrS);
}

//--------------------------------------------------------------------------------------------
#define DIFF_MAX_LIMIT		0.90
#define DIFF_MIN_LIMIT		0.10

void aci_se_init(ACISE *v)
{	
	Float tr, tc;

	tr = _IQdiv(aci_ad.lr + aci_ad.lm, aci_ad.rr);
	tc = _IQdiv(1.0, _IQmpy(2.0*M_PI, v->fc));
	
	v->K[0] = _IQdiv(1.0, _IQmpy(aci_base.wb, tr));
	v->K[1] = (1.0, _IQmpy(aci_base.fb, aci_base.ts));
	v->K[2] = _IQdiv(tc, tc + aci_base.ts);
	v->K[3] = _IQdiv(aci_base.ts, tc + aci_base.ts);
}

void aci_se_calc(ACISE *v)
{
	LgInt WSlip, WSyn;
	
	// Slip computation
	v->SquaredPsi = _IQmpy(v->PsiDrS, v->PsiDrS) + _IQmpy(v->PsiQrS, v->PsiQrS);
	
	WSlip = _IQmpy(v->K[0], (_IQmpy(v->PsiDrS, v->IQsS) - _IQmpy(v->PsiQrS, v->IDsS)));
	if (WSlip != 0) WSlip = _IQdiv(WSlip, v->SquaredPsi);
	
	// Synchronous speed computation
	if ((v->ThetaFlux < DIFF_MAX_LIMIT) && (v->ThetaFlux > DIFF_MIN_LIMIT))
		   WSyn = _IQmpy(v->K[1], (v->ThetaFlux - v->OldThetaFlux));
	else  WSyn = v->WPsi;

	// Low-pass filter
	v->WPsi = _IQmpy(v->K[2], v->WPsi) + _IQmpy(v->K[3], WSyn);

	v->OldThetaFlux = v->ThetaFlux;

	// Limit the estimated speed between -1 and 1 per-unit
	v->WrHat = _IQsat((v->WPsi - (WSlip)), (1.0), (-1.0));
}



