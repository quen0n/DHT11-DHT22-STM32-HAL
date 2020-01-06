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
  GPIO_InitStruct.Pull = GPIO_NOPULL;						//Без подтяжки
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //Высокая скорость работы порта
  HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
}

static void goToInput(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //Настройка порта на вход 
  GPIO_InitStruct.Pin = DHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL; 					//Без подтяжки
  HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
}

DHT_data DHT_getData(void) {
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
	//Ожидание спада
	while(getLine());
	//Ожидание подъёма
	while(!getLine());
	//Ожидание спада
	while(getLine());
	
	/* Чтение ответа от датчика */
	//Чтение 16-ти бит влажности 
	uint16_t hum = 0;
	for(uint8_t i = 15; i != 255; i--) {
		uint32_t hT = 0, lT = 0;
		//Пока линия в низком уровне, инкремент переменной lT
		while(!getLine()) lT++;
		//Пока линия в высоком уровне, инкремент переменной hT
		while(getLine()) hT++;
		//Если hT больше lT, то пришла единица
		if(hT > lT) hum |= (1<<i);
	}
	//Чтение 16-ти бит температуры 
	uint16_t temp = 0;
	for(uint8_t i = 15; i != 255; i--) {
		uint32_t hT = 0, lT = 0;
		//Пока линия в низком уровне, инкремент переменной lT
		while(!getLine()) lT++;
		//Пока линия в высоком уровне, инкремент переменной hT
		while(getLine()) hT++;
		//Если hT больше lT, то пришла единица
		if(hT > lT) temp |= (1<<i);
	}
	//Чтение 8-ми бит контрольной суммы 
	uint16_t checkSum = 0;
	for(uint8_t i = 7; i != 255; i--) {
		uint32_t hT = 0, lT = 0;
		//Пока линия в низком уровне, инкремент переменной lT
		while(!getLine()) lT++;
		//Пока линия в высоком уровне, инкремент переменной hT
		while(getLine()) hT++;
		//Если hT больше lT, то пришла единица
		if(hT > lT) checkSum |= (1<<i);
	}
	
	/* Проверка целостности данных */
	
	/* Конвертация и возврат полученных значений */
	DHT_data data = {(float)hum*0.1f,(float)temp*0.1f};
	return data;	
}



/*static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO Ports Clock Enable 
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //Configure GPIO pin Output Level 
  HAL_GPIO_WritePin(DHT_PIN_GPIO_Port, DHT_PIN_Pin, GPIO_PIN_SET);

  //Configure GPIO pin : DHT_PIN_Pin 
  GPIO_InitStruct.Pin = DHT_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(DHT_PIN_GPIO_Port, &GPIO_InitStruct);


	
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO Ports Clock Enable 
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //Configure GPIO pin : PA0 
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
*/