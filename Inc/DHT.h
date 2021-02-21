#ifndef DHT_H_
#define DHT_H_

#include "main.h"

/*TODO:
 * - Возможность получать значения с разных датчиков
 * - Функция получения целочисленных значений
 */

/* Структура возвращаемых датчиком данных */
typedef struct {
	float hum;
	float temp;
} DHT_data;
/* Тип используемого датчика */
typedef enum {
	DHT11,
	DHT22
} DHT_type;

/* Настройки */
#define DHT_Port 	DHT11_GPIO_Port		//Группа линии данных
#define DHT_Pin 	DHT11_Pin 			//Пин линии данных
#define DHT_PullUp 	1					//Нужно ли включать внутреннюю подтяжку пина к питанию
#define DHT_timeout 			10000	//Количество итераций, после которых функция вернёт пустые значения
#define DHT_POLLING_CONTROL		1		//Включение проверки частоты опроса датчика
#define DHT_POLLING_INTERVAL	2000	//Через какой промежуток времени производить повторный опрос датчика


/* Прототипы функций */
DHT_data DHT_getData(DHT_type t); //Получить данные с датчика

#endif
