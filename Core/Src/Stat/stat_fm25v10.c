#include "main.h"
#include "stat_fm25v10.h"
#include "csl_memory.h"


#define WRSR		0x01
#define WRITE		0x02
#define READ		0x03
#define WRDI		0x04
#define RDSR		0x05
#define WREN 		0x06



#define STATUS_MASK				0xBF

#define ADDRESS_HIGH_MASK		        0x10000

Byte e1 = 0;
Byte e2 = 0;

void FM25V10_Init(TFM25V10 *p)
{
   
	p->Buffer	= Null;					// Указатель на буфер данных
	p->RdData	= 0;					// Текущий считанный байт
	p->WrData	= 0;					// Текущий записанный байт
	p->State	= 0;					// Текущий шаг выполнения
	p->Error	= false;				// Флаг ошибки в работе памяти
	p->RetryTimer	= 0;				// Таймер повторов перезаписи
	p->RetryCount	= EEPROM_RETRY;		// Количество повторов перезаписи
	p->BusyTimer	= 0;				// Таймер окончания записи
	if (FM25V10_ReadStatus(p))
        p->Error = TRUE;	
          
         p->SN = FM25V10_ReadSN(p);
}

void FM25V10_Update(TFM25V10 *p)
{
	register Uns Tmp;

	if (!p->Func) {p->State = 0; return;}
	
	switch (++p->State)
	{
		case 1:
			if (p->Func & F_READ)    p->State = 2;						// Состояния на 1 больше, т.к. каждый проход switch'а, состояния инкрементируются
			else if (p->Func & F_WRITE)   p->State = 3;
			else {p->Func = 0; p->State = 0;}								// Если функций нет, то обнуляем State
			break;
//-----------------------------------------------------------------
		case 3:															// Чтение байта
			
			Tmp = FM25V10_ReadByte(p, p->Addr);							// Считываем байт по указанному адресу во временную переменную

			if (!(p->Func & F_WRITE))									// Функция записи отсутствует, значит читаем байт
			{
				if (!(p->Addr & 0x1)) p->RdData = Tmp;					// Текущий адрес четный, значит младшие биты. Записываем в RdData
				else
				{
					p->RdData = (Tmp << 8) | p->RdData;					// Адрес нечетный. Биты старшие. Склеиваем младшие и старшие биты в RdData. Если не будет работать, то нужно будет сделать преведение к Uns
					*((Uns *)p->Buffer) = p->RdData;					// Приводим тип указателя к Uns и записываем данные из RdData в текущую ячейку буфера
				}
			}
			else if (Tmp != p->WrData)									// Если функция записи и считанное значение по адресу, по которому недавно проходила запись не совпадает с данными для записи
			{
				if (++p->RetryTimer < p->RetryCount) p->Func &= ~F_READ;	// Инкрементируем счетчик попыток и проверяем, если ли еще попытки. Если есть, то отсекаем функцию чтения,
																			// чтобы потом по статусу проверить готовность устройства и повторить цикл записи
				else 
				{
					p->Error = TRUE; p->Func = 0;								// Если попытки на перезапись кончились, то выставляем ошибку
				}						
				p->State = 0;
				break;														// Выходим из case
			}
			else {p->RetryTimer = 0; p->Func &= ~F_READ;}					// В остальных случаях обнуляем счетчик попыток и отсекаем функцию чтения
			
			if (p->Count > 1)												// Есть еще байты для записи
			{
				if (p->Buffer && (p->Addr & 0x1))	p->Buffer++;			// Проверяем, что буфер не пустой и что адрес нечетный (первыми считываются младшие биты, затем старшие)
				p->Addr++;
				p->Count--;
			}
			else
			{
				p->RetryTimer = 0;
				p->Count = 0;
				p->Func = 0;
			}

			p->State = 0;
			break;
//-------------------------------------------------------------------------
		case 4:
			if (!p->Buffer) 
                          Tmp = 0;					// Буфер пуст
			else 
                          Tmp = *((Uns *)p->Buffer);				// Записываем данные из буфера во временную переменную

			if (!(p->Addr & 0x1)) 						// Если 1 - то старшие биты, если 0, то младшие.
				 p->WrData = (Byte)(Tmp & 0xFF);		// Счет адресов идет вверх. Такого - 0011 0000 0000 быть не должно.
														// Продавливаем по маске, для отсечения старших бит и приводим тип к 8-битному
			else 
                          p->WrData = (Byte)(Tmp >> 8);			// Сдвигаем старшую часть и приводим к 8-битному типу

					// Инициализируем SPI
			FM25V10_WriteEnable(p);										// Посылаем запрос на разрешение записи
			break;
		case 5:
					// Инициализируем SPI
			FM25V10_WriteByte(p, p->Addr, p->WrData);					// Пишем байт
			p->Func |= (F_READ);										// Посылка на запись прошла, теперь необходимо проверить правильность записанных данных
																		// по статусному регистру, а так же считать записанные данные для проверки
			p->BusyTimer = 0;											// Сбрасываем таймер, т.к. будем заново проводить проверку готовности
			p->State = 0;
			break;
	}
}

Byte FM25V10_ReadStatus(TFM25V10 *p)
{
	uint8_t  Status; 
        uint8_t  Data;
        Data = RDSR;
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET); 

//	SpiWriteByte(RDSR, SPI2);                                                                                      
//	Status = SpiReadByte(SPI2);	

        HAL_SPI_Transmit(&hspi1, &Data, 1, 100);
        HAL_SPI_Receive(&hspi1, &Status, 1, 100);

//HAL_SPI_TransmitReceive(&hspi1, &Data, &Status, 1, 100);
     
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
          
        return 0;//Status & STATUS_MASK;           
}

void FM25V10_WriteStatus(TFM25V10 *p, Byte Status)
{
	uint8_t cmd[2];
          
        cmd[0] = WRSR;
        cmd[1] = Status;  
          
        HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET); 
        HAL_SPI_Transmit(&hspi1, cmd, 2, 100);  
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
}

Byte FM25V10_ReadByte(TFM25V10 *p, unsigned long Addr)
{
	uint8_t Data;
        uint8_t cmd[4];
          
         cmd[0] = READ;
         cmd[1] = (Addr & ADDRESS_HIGH_MASK) >> 16;
         cmd[2] = (Addr >> 8) & 0xFF;
         cmd[3] =  Addr & 0xFF;
  
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET); 
        //HAL_SPI_TransmitReceive(&hspi1, cmd, Data, 5, 100);   
	HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
        HAL_SPI_Receive(&hspi1, &Data, 1, 100);
            HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 

	return Data;											
}

void FM25V10_WriteByte(TFM25V10 *p, unsigned long Addr, Byte Data)
{
	
         uint8_t cmd[5];
          
         cmd[0] = WRITE;
         cmd[1] = (Addr & ADDRESS_HIGH_MASK) >> 16;
         cmd[2] = (Addr >> 8) & 0xFF;
         cmd[3] =  Addr & 0xFF; 
         cmd[4] =  Data;
           
        HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET);   
        HAL_SPI_Transmit(&hspi1, cmd, 5, 100);   
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
}

void FM25V10_WriteEnable(TFM25V10 *p)
{
	uint8_t Data = WREN;
          
        HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET); 
        HAL_SPI_Transmit(&hspi1, &Data, 1, 100);
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
}

void FM25V10_WriteDisable(TFM25V10 *p)
{
  	uint8_t Data = WRDI;
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET);        
        HAL_SPI_Transmit(&hspi1, &Data, 1, 100);    
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
}

Byte FM25V10_ReadSN(TFM25V10 *p)
{
        uint8_t  Status; 
        uint8_t  Data;
        Data = 195;
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_RESET); 

        HAL_SPI_Transmit(&hspi1, &Data, 1, 100);
        HAL_SPI_Receive(&hspi1, &Status, 1, 100);
    
	HAL_GPIO_WritePin(CS_EEPROM_GPIO_Port, CS_EEPROM_Pin, GPIO_PIN_SET); 
          
        return Status;  
}


