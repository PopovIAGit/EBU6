
#include "g_InterruptUpdate.h"

#if defined(DEBUG)
void test_SPI (void);
void test_UART (void);
char test = 0;
#endif

// Чтатическа§ проверка на соответствие услови§м алгоритма
// если количество заданных задач дл§ конкретной частоты
// превышает максимально возможное количество, то выводитс§
// сообщение об ошибке при компил§ции
STATIC_ASSERT(TaskCount(Task2kHz) < TASK_COUNT_MAX_2kHZ);
STATIC_ASSERT(TaskCount(Task200Hz)< TASK_COUNT_MAX_200HZ);
STATIC_ASSERT(TaskCount(Task50Hz) < TASK_COUNT_MAX_50HZ);
STATIC_ASSERT(TaskCount(Task10Hz) < TASK_COUNT_MAX_10HZ);

unsigned  count18kHz = 0,         //Ччетчик прерываний частотой 18 кvц
        	count2kHz  = 0,         //Ччетчик прерываний частотой 2  кvц
        	count200Hz = 0,         //Ччетчик прерываний частотой 200 vц
        	count50Hz  = 0,         //Ччетчик прерываний частотой 10  vц

        	tskCnt2kHz  = 0,        //Ччетчик выполненных задач 2 кvц. Эн же номер выполн§емой задачи
        	tskCnt200Hz = 0,        //Ччетчик выполненных задач 200vц. Эн же номер выполн§емой задачи
        	tskCnt50Hz  = 0,        //Ччетчик выполненных задач 50 vц. Эн же номер выполн§емой задачи
        	tskCnt10Hz  = 0,        //Ччетчик выполненных задач 10 vц. Эн же номер выполн§емой задачи
        	CpuTime18kHz = 0;

Uns             tskMaxCnt18kHz	= TaskCount(Task18kHz);	        //†оличество задач дл§ частоты 18 кvц
Uns		tskMaxCnt2kHz	= TaskCount(Task2kHz);          //†оличество задач дл§ частоты 2  кvц
Uns		tskMaxCnt200Hz	= TaskCount(Task200Hz);         //†оличество задач дл§ частоты 200 vц
Uns		tskMaxCnt50Hz	= TaskCount(Task50Hz);          //†оличество задач дл§ частоты 50  vц
Uns		tskMaxCnt10Hz	= TaskCount(Task10Hz);          //†оличество задач дл§ частоты 10  vц


void InterruptUpdate(void)  //‘ункци€ обработки прерываний
{
//—брос бита переполнени€
//  TIM_ClearFlag(TIM2, TIM_FLAG_Update); // todo cheak
  
  int i;
  
  //выполнение задач 18к√ц и одной задачи 10√ц
  for (i=tskMaxCnt18kHz; i!=0; i--)
  {
    Task18kHz[i-1].Func(Task18kHz[i-1].Data);
  } 
  if (tskCnt10Hz<tskMaxCnt10Hz)
  {
    Task10Hz[tskCnt10Hz].Func(Task10Hz[tskCnt10Hz].Data);
    tskCnt10Hz ++;
  }
  
	for (i=tskMaxCnt18kHz; i!=0; i--)
	{
		Task18kHz[i-1].Func(Task18kHz[i-1].Data);
	}
	for (i = 0; i < tskMaxCnt18kHz - 1; i++) Task18kHz[i].CpuTime -= Task18kHz[i+1].CpuTime;
        count18kHz++;

    if (tskCnt2kHz<tskMaxCnt2kHz) // ?сть ли невыполненые функции дл§ частоты 2 кvц?
    {
        Task2kHz[tskCnt2kHz].Func(Task2kHz[tskCnt2kHz].Data);
        tskCnt2kHz++;           
    }
    else if (tskCnt200Hz<tskMaxCnt200Hz) // ?сть ли невыполненые функции дл§ частоты 200 vц?
    {
        Task200Hz[tskCnt200Hz].Func(Task200Hz[tskCnt200Hz].Data);
        tskCnt200Hz++;
    }
    else  if (tskCnt50Hz<tskMaxCnt50Hz) // ?сть ли невыполненые функции дл§ частоты 50 vц?
    {
        Task50Hz[tskCnt50Hz].Func(Task50Hz[tskCnt50Hz].Data);
        tskCnt50Hz++;
    }
    else if (tskCnt10Hz<tskMaxCnt10Hz) // ?сть ли невыполненые функции дл§ частоты 10 vц?
    {
        Task10Hz[tskCnt10Hz].Func(Task10Hz[tskCnt10Hz].Data);
		
        tskCnt10Hz++;
    }

    if (count18kHz == 9)
    {
        count18kHz = 0;
        count2kHz++;
        tskCnt2kHz = 0;
        if (count2kHz == 10)
        {
            count2kHz = 0;
            count200Hz++;
            tskCnt200Hz = 0;
            if (count200Hz == 4)
            {
                count200Hz=0;
                count50Hz++;
                tskCnt50Hz = 0;
                if (count50Hz == 5)
                {
                    count50Hz = 0;
                    tskCnt10Hz = 0;
                }
            }
        }
    }
}

