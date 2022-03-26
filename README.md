# DHT
Лучшая простая библиотека для работы с датчиками DHT11/DHT22/AM2302/AM2301 для STM32 с использованием HAL.
## Возможности
- Поддержка датчиков DHT11/DHT22/AM2302/AM2301
- Автоматическая проверка контрольной суммы
- Не требуются таймеры и прерывания
- Возможность опроса нескольких датчиков
- Автоматический контроль частоты опроса 0.5 Гц для DHT11 и 1 Гц для DHT22  
- Возможность включения внутреннего подтягивающего резистора (требуется для работы с датчиками без обвязки)
## Использование 
1) Настройте проект в CubeMX. Настройте порт к которому подключен датчик на ввод. Сгенерируйте проект.
2) Скопируйте ```DHT.h``` и ```DHT.c``` в папки проекта ```Inc``` и ```Src``` соответственно. При необходимости подключите файлы в IDE. 
3) Создайте переменную типа ```DHT_sensor```, указав в нём порт, тип датчика, и, если у вас датчик без обвязки, включите подтяжку линии данных.
4) Для получения данных с датчика вызовите ```DHT_getData()``` указав в аругменте переменную датчика.
## Примеры кода
### Вывод данных датчика DHT11 в UART 
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  //Создание объекта дачика DHT11, подключенного к PB4, подтяжка линии данных выключена  
  static DHT_sensor livingRoom = {GPIOB, GPIO_PIN_4, DHT11};
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
![DHT11](https://sun9-81.userapi.com/impf/ruE2LeVxppeuEgoce5fyE3fJw3Y5JjJ8qtbzeA/Pr0FDnGw8a8.jpg?size=549x129&quality=96&sign=f288597d3c3bd288e0b3a62abf632d3f&type=album) 
### Вывод данных датчика DHT22 в UART с активацией подтяжки линии данных
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  //Создание объекта дачика DHT22, подключенного к PC1, подтяжка линии данных включена  
  static DHT_sensor bedRoom = {GPIOC, GPIO_PIN_1, DHT22, GPIO_PULLUP};
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
![DHT22](https://sun9-82.userapi.com/impf/CjLCFneoHTeIwtnqngnUUpAXOM3M0a6cUfs87A/KS-KxXq20Wg.jpg?size=549x129&quality=96&sign=a80b6ce8ee98102baa69043d7cca1665&type=album)
   
UPD: [Люди подсказывают](https://github.com/Quen0n/DHT/issues/1) о необходимости флага ```-u _printf_float``` в опциях линкера для печати чисел с плавающей точкой. 
### Вывод данных датчиков DHT11 и DHT22 в UART
```c
#include "DHT.h"
#include <stdio.h>
#include <string.h>
...
int main(void) {
  ...
  DHT_sensor livingRoom = {GPIOB, GPIO_PIN_4, DHT11, GPIO_NOPULL};
  DHT_sensor bedRoom = {GPIOC, GPIO_PIN_1, DHT22, GPIO_PULLUP};
  ...
  while(1) {
    //Буффер для печати текста
    char msg[40];
    //Получение данных с датчика DHT11
    DHT_data d = DHT_getData(&livingRoom);
    //Печать данных в буффер
    sprintf(msg, "\fLiving room: Temp %d°C, Hum %d%%\r\n", (uint8_t)d.temp, (uint8_t)d.hum);
    //Отправка текста в UART
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
    //Получение данных с датчика DHT22
    d = DHT_getData(&bedRoom);
    //Печать данных в буффер
    sprintf(msg, "Bed room: Temp %2.1f°C, Hum %2.1f%%", d.temp, d.hum);
    //Отправка текста в UART
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
  }
}
```
![DHT22](https://sun9-63.userapi.com/impf/Z93G0lg8AV7Eku4lpz94AFSSt2yqBWR5c7gf2w/Wb7tPIKMmxE.jpg?size=549x129&quality=96&sign=18342a3b5366c2be09813588146ccac1&type=album)

## Обратная связь, нужна помощь?
Предложения или найденные баги можно сообщить в Telegram http://t.me/quen0n, ВКонтакте https://vk.com/quenon или на почту rainbowkiwifox@gmail.com
