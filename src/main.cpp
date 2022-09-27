/** Автоматическая работа вентилятора с учетом температуры и влажности всасываемого воздуха
 * После включения питания вентилятор отключается. Дальнейшая работа зависит от показаний датчика влажности и температуры
 * 
 * TODO: Аварийный режим - включение и выключение по таймеру (например, при отсутствии датчика или wifi) 
 * TODO: Поднятие точки доступа и простого web-сервера для настройки WiFi в случае певого подключения
 * TODO: Запись настроек в EEPROM для последующего их чтения при старте.
 * TODO: Обновление прошивки по воздуху (OTA)
 */ 

/* Подключение библиотек */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


/* Подключение чувствительной информации */
#include <creditians.h>

/* Объявление переменных */
unsigned long lastUpdate;                     // Время последней проверки параметров работы вентилятора и состояния датчиков
bool needNewMessage, sensorAvailable, controlAvailable;       // Доступные режимы


/** Инициализация */
ESP8266WiFiMulti wifiMulti;



void setup() {
  delay(INIT_DELAY);
  Serial.begin(115200);
  delay(200);
  Serial.println("Инициализация.");
  Serial.printf("(%ld) \t Инициализация\n", millis()); 
  delay(200);
  Serial.printf("(%ld) \t Начинаем инициализацию реле\n", millis());
  pinMode(RELAY_PIN, OUTPUT);
  delay(200);
  Serial.printf("(%ld) \t Реле в состоянии %s.\n", millis(), (digitalRead(RELAY_PIN) != 0) ?  "OFF" : "ON");
  delay(200);

  Serial.printf("(%ld) \t Ожидание раскрутки вентилятора (%d мсек).\n", millis(),WARM_TIME);
  delay(WARM_TIME);                 // Начальная раскрутка вентилятора
  
  Serial.printf("(%ld) \t Раскрутка завершена. Выключаем реле.\n", millis());
  digitalWrite(RELAY_PIN, HIGH);    // выключение вентилятора
  delay(200);
  Serial.printf("(%ld) \t Реле в состоянии %s.\n", millis(), (digitalRead(RELAY_PIN) != 0) ?  "OFF" : "ON");
  Serial.println("Инициализация реле закончена.");
  
  
  
  // Инициализация wifi
  Serial.println("Поиск сети");
  Serial.printf("Connecting to %s.\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Инициализация завершена.");

  Serial.print("Переменная  controlAvailable = ");
  Serial.println(controlAvailable);
  Serial.print("Переменная  sensorAvailable = ");
  Serial.println(sensorAvailable);
}

void loop() {
  
  // Опрашивать состояние не чаще, чем CHECK_PERIOD
  if(millis() - lastUpdate > CHECK_PERIOD){
    
    // /** Проверить условия и если нужно - включить вентилятор.
    //  * Включить вентилятор если включен автономный режим, вентилятор выключен и общее время работы больше FAN_DELAY
    //  * Дальнейшие проверки не проводить. 
    //  * */
    // if(!controlAvailable && (millis() >= FAN_DELAY) && (digitalRead(RELAY_PIN) == HIGH) ) {
    //     digitalWrite(RELAY_PIN,LOW);
    //     Serial.println("Система в автономном режиме. Вентилятор включен по прошествии FAN_DELAY.");
    //     return;
    // }
    
    // // Проверить статус соединения wifi и отключить автономный режим если соединение есть
    // if((WiFi.status() == WL_CONNECTED) && !controlAvailable){  
    //   Serial.println("WiFi подключен, IP адрес: " + WiFi.localIP().toString() + ". Система переходит в управляемый режим.");
    //   controlAvailable = true;
    //   return;
    // } 
    

    lastUpdate = millis();



    // второй вариант - только автономный режим.
    if(!controlAvailable){
      // проверить таймер
      if((millis() >= FAN_DELAY) && (digitalRead(RELAY_PIN) == HIGH)){
        digitalWrite(RELAY_PIN,LOW);
        Serial.println("Система в автономном режиме. Вентилятор включен по прошествии FAN_DELAY.");
        return;  
      }
      
      // проверить, не появилась ли сеть
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("WiFi подключен, IP адрес: " + WiFi.localIP().toString() + ". Система переходит в управляемый режим.");
        controlAvailable = true;
        return;
      }
    } else {
      /** Проверить, не отвалилась ли сеть, 
       * если отвалилась - перейти в автономный режим
       */ 
      if(WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi потерян. Система переходит в автономный режим.");
        controlAvailable = true;
        return;
      }
      /** Проверить показания датчика в управляемом режиме
       *  Сравнить их с пороговыми значениями
       *  Установить режим вентилятора
       */
      Serial.println("Система в управляемом режиме. Вентилятор управляется датчиком.");

    }



  }
  

}




