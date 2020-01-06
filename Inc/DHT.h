#ifndef DHT_H_
#define DHT_H_

/*TODO:
- Включение/выключение внутренней подтяжки линии
- Выбор типа датчика
- Защита от зависания
- Поддержка отрицательных температур
*/

#include "main.h"

/* Структура возвращаемых датчиком данных */
typedef struct {
	float hum;
	float temp;
} DHT_data;

/* Настройки */
#define DHT_Port 	GPIOA				//Группа линии данных
#define DHT_Pin 	GPIO_PIN_0 	//Пин линии данных

/* Прототипы функций */
DHT_data DHT_getData(void); //Получить данные с датчика


#endif