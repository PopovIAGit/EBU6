/*======================================================================
»м€ файла:          core_TorqueObserver.c
јвтор:				ѕопов ».ј.
¬ерси€ файла:		1.00
ƒата изменени€:		16.04.2014
ќписание:
модуль управлени€ задвижкой
======================================================================*/

#include "g_Core.h"
#include "peref.h"

// фиксированные точки дл€ сн€ти€ момента
//Int VoltArray[CUB_COUNT1] 	= {160, 190,  220,  250};
Int VoltArray[CUB_COUNT1] 	= 220;
                                  //1   15    21   25   30   35   40  45    50   55 
Int CurrArray[CUB_COUNT2] 	= {400, 500, 550, 600, 650, 700, 750, 800, 850, 900};		// добавил 3000 PIA 14.11.17

Int AnUIArray[CUB_COUNT2] 	= {25, 35, 50, 65, 70,  80};	// добавил 70 	PIA 14.11.17

void CubInit(TCubStr *p, TCubConfig *Cfg)	//инициализаци€ куба
{
	register TCubPoint *Pt;
	register Uns i, j, Tmp;

	p->InputX = Cfg->X_Value;//среднее напр€жение
	p->InputY = Cfg->Y_Value;//средний ток в процентах от номинального

	p->Min.X  = 0x7FFF;
	p->Max.X  = 0x8000;
	p->Min.Y  = 0x7FFF;
	p->Max.Y  = 0x8000;

	for (i=0; i < CUB_COUNT1; i++)
	{
		Tmp = Cfg->X_Array[i];
		for (j=0; j < CUB_COUNT2; j++)
		{
			Pt = &p->Points[i][j];
			Pt->X = Tmp;
			Pt->Y = Cfg->Y_Array[j];

			if (Pt->X < p->Min.X) p->Min.X = Pt->X;
			if (Pt->X > p->Max.X) p->Max.X = Pt->X;
			if (Pt->Y < p->Min.Y) p->Min.Y = Pt->Y;
			if (Pt->Y > p->Max.Y) p->Max.Y = Pt->Y;
		}
	}
}


void CubRefresh(TCubStr *p, TCubArray *Array)	//
{
	register TCubPoint *Pt;
	register Uns i, j;

	for (i=0; i < CUB_COUNT1; i++)
	{
		for (j=0; j < CUB_COUNT2; j++)
		{
			Pt = &p->Points[i][j];
			Pt->Z = Array->Data[i][j];
		}
	}
}

void CubCalc(TCubStr *p)
{
    int16 A0, A1, B0, B1;

    	// контроль входных данных на случай если они выпали за пределы куба
    	p->Input.X = *p->InputX;
    	p->Input.Y = *p->InputY;

    	if (p->Input.X < p->Min.X) p->Input.X = p->Min.X;
    	if (p->Input.X > p->Max.X) p->Input.X = p->Max.X;
    	if (p->Input.Y < p->Min.Y) p->Input.Y = p->Min.Y;
    	if (p->Input.Y > p->Max.Y) p->Input.Y = p->Max.Y;

    	// поиск квадрата
    	for (p->Num1=0; p->Num1 < CUB_COUNT1-1; p->Num1++)
    		if (p->Input.X < p->Points[p->Num1+1][0].X) break;
    	for (p->Num2=0; p->Num2 < CUB_COUNT2-1; p->Num2++)
    		if (p->Input.Y < p->Points[p->Num1][p->Num2+1].Y) break;

    	// расчет точки R
    	A0 = p->Points[p->Num1][p->Num2].X;
    	A1 = p->Points[p->Num1+1][p->Num2].X;
    	B0 = p->Points[p->Num1][p->Num2].Z;
    	B1 = p->Points[p->Num1+1][p->Num2].Z;

    	p->PtR  = B0 + (int16)((float32)(p->Input.X - A0) * (B1 - B0) / (A1 - A0));

    	// расчет точки T
    	A0 = p->Points[p->Num1][p->Num2+1].X;
    	A1 = p->Points[p->Num1+1][p->Num2+1].X;
    	B0 = p->Points[p->Num1][p->Num2+1].Z;
    	B1 = p->Points[p->Num1+1][p->Num2+1].Z;

    	p->PtT  = B0 + (int16)((float32)(p->Input.X - A0) * (B1 - B0) / (A1 - A0));

    	// последний расчет
    	A0 = p->Points[p->Num1][p->Num2].Y;
    	A1 = p->Points[p->Num1][p->Num2+1].Y;
    	B0 = p->PtR;
    	B1 = p->PtT;

    	p->Output  = B0 + (int16)((float32)(p->Input.Y - A0) * (B1 - B0) / (A1 - A0));
}

// »нициализаци€
void Core_TorqueInit(TTorqObs *p)
{

	p->TqCurr.X_Value = (Int *)&g_Ram.HideParam.Umid;// - VoltTmp;
	p->TqCurr.X_Array = VoltArray;
	p->TqCurr.Y_Value = (Int *)&g_Ram.HideParam.Imidpr;
	p->TqCurr.Y_Array = CurrArray;

	// втора€ поверхность U и AngFi - малые токи
/*	p->TqAngUI.X_Value = (Int *)&g_Ram.HideParam.Umid;
	p->TqAngUI.X_Array = VoltArray;
	p->TqAngUI.Y_Value = (Int *)&g_Ram.ramGroupA.AngleUI;
	p->TqAngUI.Y_Array = AnUIArray;*/

	// «абираем средний ток
	p->Imidpr 	= &g_Ram.HideParam.Imidpr;
	p->TorqueSetPr 	= &g_Core.MotorControl.TorqueSetPr;
	p->TransCurr 	= &g_Ram.HideParam.TransCurr;

	// »нициализируем поверхности
	CubInit(&p->Cub1, &p->TqCurr);		 // инициализаци€ структуры куба дл€ расчета момента
	//CubInit(&p->Cub2, &p->TqAngUI);

	// »нициализируем фильтр момента
	peref_ApFilter3Init(&p->Trqfltr, (Uns)PRD_18KHZ, 1);
}

// –асчет момента
void Core_TorqueCalc(TTorqObs *p)
{
		register TCubStr *Cub;	//
		//register Int Tmp;
		register Int Add;		// ручной контроль индикации момента
		//p->ObsEnable=true;
		if (!p->ObsEnable) {p->Indication = 0; return;} //если выключен расчет момента то индикаци€ 0 и закрываем тиристоры и выходим

		//Cub = (*p->Imidpr >= *p->TransCurr) ? &p->Cub1 : &p->Cub2;  // выбераем по какому кубу работаем дл€ маленьких или больших токов
                Cub = &p->Cub1; 
		CubCalc(Cub);	// считаем выбранный куб

	/*	if (g_Core.MotorControl.RequestDir == -1)
		{
		    if      (*p->TorqueSetPr < 30) 	Add = g_Ram.ramGroupC.CorrClose30Trq;		// ƒобавил PIA 09.10.2012
		    else if (*p->TorqueSetPr < 40)	Add = g_Ram.ramGroupC.CorrClose40Trq;
		    else if (*p->TorqueSetPr < 60)	Add = g_Ram.ramGroupC.CorrClose60Trq;		// корректировка индикации момента дл€ +- изменени€ времени перехода на поверхность упора
		    else if (*p->TorqueSetPr < 80)	Add = g_Ram.ramGroupC.CorrClose80Trq;
		    else if (*p->TorqueSetPr < 110)	Add = g_Ram.ramGroupC.CorrClose110Trq;
		}
		else if (g_Core.MotorControl.RequestDir == 1)
		{
		    if 	    (*p->TorqueSetPr < 30) 	Add = g_Ram.ramGroupC.CorrOpen30Trq;		// ƒобавил PIA 09.10.2012
		    else if (*p->TorqueSetPr < 40)	Add = g_Ram.ramGroupC.CorrOpen40Trq;
		    else if (*p->TorqueSetPr < 60)	Add = g_Ram.ramGroupC.CorrOpen60Trq;		// корректировка индикации момента дл€ +- изменени€ времени перехода на поверхность упора
		    else if (*p->TorqueSetPr < 80)	Add = g_Ram.ramGroupC.CorrOpen80Trq;
		    else if (*p->TorqueSetPr < 110)	Add = g_Ram.ramGroupC.CorrOpen110Trq;
		}
		else if (g_Core.MotorControl.RequestDir == 0) Add = 0;*/
                Add = 0; // del

		p->Trqfltr.Input = (Float)Cub->Output; // фильтруем значение момента
		peref_ApFilter3Calc(&p->Trqfltr);

		p->Tmp =((Uns)(p->Trqfltr.Output));

		if (p->Tmp < TORQ_MIN_PR) p->Tmp = TORQ_MIN_PR;	// провер€ем на вхождение в зону от
		if (p->Tmp > TORQ_MAX_PR) p->Tmp = TORQ_MAX_PR;   // 10 до 110 %

		if (abs(Add) > p->Tmp) Add = 0;

		//PU0ToValue(Tmp, p->TorqueMax);// переводим проценты в Ќм относительно максимального ћ
		p->Indication = ((Uns)((((LgUns)p->Tmp + Add) * p->TorqueMax) / 100));
}


