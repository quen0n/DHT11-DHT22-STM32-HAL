#include "DHT.h"

#define lineDown() 	HAL_GPIO_WritePin(DHT_Port, DHT_Pin, GPIO_PIN_RESET)
#define lineUp()		HAL_GPIO_WritePin(DHT_Port, DHT_Pin, GPIO_PIN_SET)
#define getLine()		(HAL_GPIO_ReadPin(DHT_Port, DHT_Pin) == GPIO_PIN_SET)

static void goToOutput(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  //По умолчанию на линии высокий уровень
  HAL_GPIO_WritePin(DHT_Port, DHT_Pin, GPIO_PIN_SET);

  //Настройка порта на выход 
  GPIO_InitStruct.Pin = DHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 	//Открытый сток
	#if DHT_PullUp == 1
  GPIO_InitStruct.Pull = GPIO_PULLUP;						//Подтяжка к питанию
	#else 
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//Без подтяжки
	#endif
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //Высокая скорость работы порта
  HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
}

static void goToInput(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //Настройка порта на вход 
  GPIO_InitStruct.Pin = DHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	#if DHT_PullUp == 1
  GPIO_InitStruct.Pull = GPIO_PULLUP;						//Подтяжка к питанию
	#else 
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//Без подтяжки
	#endif
  HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
}

DHT_data DHT_getData(DHT_type t) {
	DHT_data data = {0.0f, 0.0f};
	
	/* Запрос данных у датчика */
	//Перевод пина "на выход"
	goToOutput();
	//Опускание линии данных на 15 мс
	lineDown();
	HAL_Delay(15);
	//Подъём линии, перевод порта "на вход"
	lineUp();
	goToInput();
	
	/* Ожидание ответа от датчика */
	uint16_t timeout = 0;
	//Ожидание спада
	while(getLine()) {
		timeout++;
		if (timeout > DHT_timeout) return data;
	}
	timeout = 0;
	//Ожидание подъёма
	while(!getLine()) {
		timeout++;
		if (timeout > DHT_timeout) return data;
	}
	timeout = 0;
	//Ожидание спада
	while(getLine()) {
		timeout++;
		if (timeout > DHT_timeout) return data;
	}
	
	/* Чтение ответа от датчика */
	uint8_t rawData[5] = {0,0,0,0,0};
	for(uint8_t a = 0; a < 5; a++) {
		for(uint8_t b = 7; b != 255; b--) {
			uint32_t hT = 0, lT = 0;
			//Пока линия в низком уровне, инкремент переменной lT
			while(!getLine()) lT++;
			//Пока линия в высоком уровне, инкремент переменной hT
			while(getLine()) hT++;
			//Если hT больше lT, то пришла единица
			if(hT > lT) rawData[a] |= (1<<b);
		}
	}
	/* Проверка целостности данных */
	if((uint8_t)(rawData[0] + rawData[1] + rawData[2] + rawData[3]) == rawData[4]) {
		//Если контрольная сумма совпадает, то конвертация и возврат полученных значений
		if (t == DHT22) {
			data.hum = (float)(((uint16_t)rawData[0]<<8) | rawData[1])*0.1f;
			data.temp = (float)(((uint16_t)rawData[2]<<8) | rawData[3])*0.1f;
		}
		if (t == DHT11) {
			data.hum = (float)rawData[0];
			data.temp = (float)rawData[2];;
		}
	}
	
	return data;	
}