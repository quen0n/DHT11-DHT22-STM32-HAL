# DHT
Лучшая простая библиотека для работы с датчиками DHT11/DHT22/AM2302/AM2301 для STM32 с использованием HAL.
## Возможности
- Поддержка датчиков DHT11/DHT22/AM2302/AM2301
- Автоматическая проверка контрольной суммы
- Не требуются таймеры и прерывания
- Поддержка опроса нескольких датчиков
- Автоматический контроль частоты опроса 0.5 Гц для DHT11 и 1 Гц для DHT22  
- Возможность включения внутреннего подтягивающего резистора (требуется для работы с датчиками без обвязки)
## Использование 
1) Настройте проект в CubeMX. Настройте порт к которому подключен датчик на ввод. Сгенерируйте проект.
2) Скопируйте ```DHT.h``` и ```DHT.c``` в папки проекта ```Inc``` и ```Src``` соответственно. При необходимости подключите файлы в IDE. 
3) Подключите ```DHT.h``` в нужном файле и для получения данных с датчика вызовите ```DHT_getData()``` указав в аругменте данные датчика.
## Примеры кода
### Вывод данных датчика DHT11 в UART 
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  //Создание объекта дачика DHT11, подключенного к PB4  
  static DHT_sensor livingRoom = {GPIOB, GPIO_PIN_4, DHT11, 0};
  ...
  while(1) {
    //Буффер для печати текста
    char msg[40]; 
    //Получение данных с датчика
    DHT_data d = DHT_getData(&livingRoom); 
    //Печать данных в буффер
    sprintf(msg, "\fLiving room: Temp %d°C, Hum %d%%", (uint8_t)d.temp, (uint8_t)d.hum);
    //Отправка текста в UART
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
  }
}
```
![DHT11](https://cloud.quenon.ru/index.php/s/TI4SKbJyE0U0clw/download) 
### Вывод данных датчика DHT22 в UART с активацией подтяжки линии данных
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  //Создание объекта дачика DHT22, подключенного к PC1  
  static DHT_sensor bedRoom = {GPIOC, GPIO_PIN_1, DHT22, 1};
  ...
  while(1) {
    //Буффер для печати текста
    char msg[40]; 
    //Получение данных с датчика
    DHT_data d = DHT_getData(&bedRoom); 
    //Печать данных в буффер
    sprintf(msg, "\fBed room: Temp %2.1f°C, Hum %2.1f%%", d.temp, d.hum);
    //Отправка текста в UART
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
  }
}
```
![DHT22](https://cloud.quenon.ru/index.php/s/25odgB7w1qr2LFo/download)
   
UPD: [Люди подсказывают](https://github.com/Quen0n/DHT/issues/1) о необходимости флага ```-u _printf_float``` в опциях линкера для печати чисел с плавающей точкой. 
### Вывод данных датчиков DHT11 и DHT22 в UART
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  DHT_sensor livingRoom = {GPIOB, GPIO_PIN_4, DHT11, 0};
  DHT_sensor bedRoom = {GPIOC, GPIO_PIN_1, DHT22, 1};
  ...
  while(1) {
    char msg[40];
    DHT_data d = DHT_getData(&livingRoom);
    sprintf(msg, "\fLiving room: Temp %d°C, Hum %d%%\r\n", (uint8_t)d.temp, (uint8_t)d.hum);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
    d = DHT_getData(&bedRoom);
    sprintf(msg, "Bed room: Temp %2.1f°C, Hum %2.1f%%", d.temp, d.hum);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
  }
}
```
![DHT22](https://cloud.quenon.ru/index.php/s/fDJGpWkdjmOuTrI/download)

## Обратная связь, нужна помощь?
Предложения или найденные баги можно сообщить в Telegram http://t.me/quen0n, ВКонтакте https://vk.com/quenon или на почту rainbowkiwifox@gmail.com
