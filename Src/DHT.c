#include "DHT.h"

#define lineDown() 		HAL_GPIO_WritePin(sensor->DHT_Port, sensor->DHT_Pin, GPIO_PIN_RESET)
#define lineUp()		HAL_GPIO_WritePin(sensor->DHT_Port, sensor->DHT_Pin, GPIO_PIN_SET)
#define getLine()		(HAL_GPIO_ReadPin(sensor->DHT_Port, sensor->DHT_Pin) == GPIO_PIN_SET)
#define Delay(d)		HAL_Delay(d)

static void goToOutput(DHT_sensor *sensor) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  //По умолчанию на линии высокий уровень
  lineUp();

  //Настройка порта на выход 
  GPIO_InitStruct.Pin = sensor->DHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 	//Открытый сток
  if(sensor->pullUp == 1) {
	  GPIO_InitStruct.Pull = GPIO_PULLUP;						//Подтяжка к питанию
  } else {
	  GPIO_InitStruct.Pull = GPIO_NOPULL;						//Без подтяжки
  }

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //Высокая скорость работы порта
  HAL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

static void goToInput(DHT_sensor *sensor) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //Настройка порта на вход 
  GPIO_InitStruct.Pin = sensor->DHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  if(sensor->pullUp == 1) {
	  GPIO_InitStruct.Pull = GPIO_PULLUP;						//Подтяжка к питанию
  } else {
	  GPIO_InitStruct.Pull = GPIO_NOPULL;						//Без подтяжки
  }
  HAL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

DHT_data DHT_getData(DHT_sensor *sensor) {
	DHT_data data = {0.0f, 0.0f};
	
	#if DHT_POLLING_CONTROL == 1
	/* Ограничение по частоте опроса датчика */
	//Определение интервала опроса в зависимости от датчика
	uint16_t pollingInterval;
	if (sensor->type == DHT11) {
		pollingInterval = DHT_POLLING_INTERVAL_DHT11;
	} else {
		pollingInterval = DHT_POLLING_INTERVAL_DHT22;
	}

	//Если частота превышена, то возврат последнего удачного значения
	if (HAL_GetTick()-sensor->lastPollingTime < pollingInterval) {
		data.hum = sensor->lastHum;
		data.temp = sensor->lastTemp;
		return data;
	}
	sensor->lastPollingTime = HAL_GetTick();
	#endif

	/* Запрос данных у датчика */
	//Перевод пина "на выход"
	goToOutput(sensor);
	//Опускание линии данных на 15 мс
	lineDown();
	Delay(15);
	//Подъём линии, перевод порта "на вход"
	lineUp();
	goToInput(sensor);
	
	/* Ожидание ответа от датчика */
	uint16_t timeout = 0;
	//Ожидание спада
	while(getLine()) {
		timeout++;
		if (timeout > DHT_TIMEOUT) return data;
	}
	timeout = 0;
	//Ожидание подъёма
	while(!getLine()) {
		timeout++;
		if (timeout > DHT_TIMEOUT) return data;
	}
	timeout = 0;
	//Ожидание спада
	while(getLine()) {
		timeout++;
		if (timeout > DHT_TIMEOUT) return data;
	}
	
	/* Чтение ответа от датчика */
	uint8_t rawData[5] = {0,0,0,0,0};
	for(uint8_t a = 0; a < 5; a++) {
		for(uint8_t b = 7; b != 255; b--) {
			uint32_t hT = 0, lT = 0;
			//Пока линия в низком уровне, инкремент переменной lT
			while(!getLine()) lT++;
			//Пока линия в высоком уровне, инкремент переменной hT
			timeout = 0;
			while(getLine()) hT++;
			//Если hT больше lT, то пришла единица
			if(hT > lT) rawData[a] |= (1<<b);
		}
	}
	/* Проверка целостности данных */
	if((uint8_t)(rawData[0] + rawData[1] + rawData[2] + rawData[3]) == rawData[4]) {
		//Если контрольная сумма совпадает, то конвертация и возврат полученных значений
		if (sensor->type == DHT22) {
			data.hum = (float)(((uint16_t)rawData[0]<<8) | rawData[1])*0.1f;
			//Проверка на отрицательность температуры
			if(!(rawData[2] & (1<<7))) {
				data.temp = (float)(((uint16_t)rawData[2]<<8) | rawData[3])*0.1f;
			}	else {
				rawData[2] &= ~(1<<7);
				data.temp = (float)(((uint16_t)rawData[2]<<8) | rawData[3])*-0.1f;
			}
		}
		if (sensor->type == DHT11) {
			data.hum = (float)rawData[0];
			data.temp = (float)rawData[2];;
		}
	}
	
	#if DHT_POLLING_CONTROL == 1
	sensor->lastHum = data.hum;
	sensor->lastTemp = data.temp;
	#endif

	return data;	
}
