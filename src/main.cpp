/** Автоматическая работа вентилятора с учетом температуры и влажности всасываемого воздуха. Управление по mqtt
 * После включения питания вентилятор отключается. Дальнейшая работа зависит от показаний датчика влажности и температуры
 * 
 * FETURES: Аварийный режим - включение и выключение по таймеру (например, при отсутствии датчика или wifi) 
 * TODO: Поднятие точки доступа и простого web-сервера для настройки WiFi в случае певого подключения
 * TODO: Запись настроек в EEPROM для последующего их чтения при старте.
 * TODO: Обновление прошивки по воздуху (OTA)
 */ 

/* Подключение библиотек */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
#include <ArduinoJson.h>

//#define DHT22
#define DEBUG

/* Подключение чувствительной информации */
#include <creditians.h>
#include <config.h>

/* Объявление переменных */
unsigned long lastUpdate;                     // Время последней проверки параметров работы вентилятора и состояния датчиков
bool sensorAvailable, controlAvailable;       // Доступные режимы
bool forceControlMode = false;                // Принудительное отключение логики в пользу полученных команд
float sensorTemperature, sensorHumidity;      // Полученные от датчика значения влажности и температуры
DynamicJsonDocument initConfig(1024);
JsonObject config;

/** Инициализация */
ESP8266WiFiMulti wifiMulti;
WiFiClient espClient;
PubSubClient client(espClient);
SimpleDHT22 sensor(SENSOR_PIN);



/** Преобразование мак-адреса 
 * @param mac Мак-адрес
 * @return Строкой
 */ 
String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

/** Эта процедура вызывается при потере соединения с брокером MQTT
 * 
 */ 
void reconnect() {
  if (!client.connected()) {
      #ifdef DEBUG
      Serial.print("Attempting MQTT connection...");
      #endif
      // Создание идентификатора client ID в формате 'esp8266-<macaddress>-<8бит_счетчика_микросекунд>
      String clientId = MQTT_USER;
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientId += macToStr(mac);
      clientId += "-";
      clientId += String(micros() & 0xff, 16);
      #ifdef DEBUG
      Serial.print("\nПопытка подключиться к mqtt брокеру по адресу "); 
      Serial.print(MQTT_SERVER); 
      Serial.print(" с идентификатором "); 
      Serial.print(clientId); 
      Serial.println(".");
      #endif
    if (client.connect(clientId.c_str())) {
      #ifdef DEBUG
      Serial.println("Подключено.");
      #endif
    } else {
      #ifdef VERBOSE
      Serial.print("Подключение не установлено ( rc="); 
      Serial.print(client.state()); 
      Serial.println(").");
      Serial.println("Повторная попытка через 5 секунд");
      #endif  
      delay(5000);

    }
  }
}

/** Эта процедура вызывается при получении даных из подписанного канала MQTT
 * @param topic MQTT топик
 * @param payload Нагрузка
 * @param lenght Длина payload
 */ 
void callback(char* topic, byte* payload, unsigned int length) {
  // Приведение к строке
  String recievedPayload;
  for (unsigned int i = 0; i < length; i++) {
    recievedPayload += (char)payload[i];
  }
  
  // Обработка полученных команд
  #ifdef DEBUG
    Serial.println("Получен топик " + String(topic) + " значение: " + recievedPayload);
  #endif
  // Команды управления вентилятором
  if(String(topic).startsWith(FAN_SPEED_CMD)){
    forceControlMode = true;
    if (recievedPayload == "1" || recievedPayload == "ON") {
      digitalWrite(RELAY_PIN, LOW);
      client.publish(FAN_SPEED, "100" );
    } 
    if (recievedPayload == "0" || recievedPayload == "OFF") {     
        digitalWrite(RELAY_PIN, HIGH);
        client.publish(FAN_SPEED, "0");
    } 
  }
}


void setup() {
  delay(INIT_DELAY);
  Serial.begin(115200);
  delay(200);
  Serial.println("Инициализация.");
  #ifdef DEBUG
    Serial.printf("(%ld) \t Инициализация\n", millis()); 
    delay(200);
    Serial.printf("(%ld) \t Начинаем инициализацию реле\n", millis());
  #endif  
  pinMode(RELAY_PIN, OUTPUT);
  delay(200);
  #ifdef DEBUG
  Serial.printf("(%ld) \t Реле в состоянии %s.\n", millis(), (digitalRead(RELAY_PIN) != 0) ?  "OFF" : "ON");
  delay(200);
  Serial.printf("(%ld) \t Ожидание раскрутки вентилятора (%d мсек).\n", millis(),WARM_TIME);
  #endif
  delay(WARM_TIME);                 // Начальная раскрутка вентилятора
  #ifdef DEBUG 
  Serial.printf("(%ld) \t Раскрутка завершена. Выключаем реле.\n", millis());
  #endif
  digitalWrite(RELAY_PIN, HIGH);    // выключение вентилятора
  delay(200);
  #ifdef DEBUG
  Serial.printf("(%ld) \t Реле в состоянии %s.\n", millis(), (digitalRead(RELAY_PIN) != 0) ?  "OFF" : "ON");
  Serial.println("Инициализация реле закончена.");
  // Инициализация wifi
  Serial.println("Поиск сети");
  Serial.printf("Connecting to %s.\n", WIFI_SSID);
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  #ifdef DEBUG
  Serial.println("Инициализация завершена.");
  Serial.print("Переменная  controlAvailable = ");
  Serial.println(controlAvailable);
  Serial.print("Переменная  sensorAvailable = ");
  Serial.println(sensorAvailable);

  Serial.println("Установка первоначальных настроек");
  #endif
  // Add an array.
  //
  //doc["sensor"] = "gps";
  initConfig["temp_control_enable"] = TEMP_CONTROL_ENABLE;
  initConfig["temp_control_off"] = TEMP_CONTROL_OFF;
  initConfig["temp_control_on"] = TEMP_CONTROL_ON;
  initConfig["hum_control_enable"] = HUM_CONTROL_ENABLE;
  initConfig["hum_control_off"] = HUM_CONTROL_OFF;
  initConfig["hum_control_on"] = HUM_CONTROL_ON;
  initConfig["warm"] = WARM_TIME;
  initConfig["period"] = CHECK_PERIOD;
  initConfig["delay"] = FAN_DELAY;
  initConfig["init"] = INIT_DELAY;


  initConfig = initConfig.createNestedArray("config");

  initConfig.add(TEMP_CONTROL_ENABLE);
  initConfig.add(TEMP_CONTROL_OFF);
  initConfig.add(TEMP_CONTROL_ON);
  initConfig.add(HUM_CONTROL_ENABLE);
  initConfig.add(HUM_CONTROL_OFF);
  initConfig.add(HUM_CONTROL_ON);
  initConfig.add(WARM_TIME);
  initConfig.add(CHECK_PERIOD);
  initConfig.add(FAN_DELAY);
  initConfig.add(INIT_DELAY);
  
  //config = initConfig.as<JsonObject>();
  // You can set a String to a JsonObject or JsonArray:
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  config["temp_control_enable"] = TEMP_CONTROL_ENABLE;
  config["temp_control_off"] = TEMP_CONTROL_OFF;
  config["temp_control_on"] = TEMP_CONTROL_ON;
  config["hum_control_enable"] = HUM_CONTROL_ENABLE;
  config["hum_control_off"] = HUM_CONTROL_OFF;
  config["hum_control_on"] = HUM_CONTROL_ON;
  config["warm"] = WARM_TIME;
  config["period"] = CHECK_PERIOD;
  config["delay"] = FAN_DELAY;
  config["init"] = INIT_DELAY;

  #ifdef DEBUG
  Serial.println("Инициализация initConfig.");
  serializeJson(initConfig, Serial);
  Serial.println();

  Serial.println("Инициализация config.");
  serializeJson(config, Serial);
  Serial.println();
  delay(3000);
  #endif

}

void loop() {
  // Опрашивать состояние не чаще, чем CHECK_PERIOD
  if(millis() - lastUpdate > CHECK_PERIOD){
    lastUpdate = millis();

    // проверить таймер, если истек - включить вентилятор
    if( !forceControlMode && (millis() >= FAN_DELAY) && (digitalRead(RELAY_PIN) == HIGH) ){
      digitalWrite(RELAY_PIN,LOW);
      #ifdef DEBUG
      Serial.println("Вентилятор включен по прошествии FAN_DELAY.");
      #endif
    }
  

    if(!controlAvailable){  
      // проверить, не появилась ли сеть, если появилась - перейти в управляемый режим
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("WiFi подключен, IP адрес: " + WiFi.localIP().toString() + ". Система переходит в управляемый режим.");
        controlAvailable = true;
        return;
      }
    } else {
      // Проверить, не отвалилась ли сеть, если отвалилась - перейти в автономный режим
      if(WiFi.status() != WL_CONNECTED) {
        #ifdef VERBOSE
        Serial.println("WiFi потерян. Система переходит в автономный режим.");
        #endif
        controlAvailable = false;
        return;
      }
      #ifdef DEBUG
      Serial.println("Система работает в управляемом режиме.");
      #endif

      /** Получить показания датчика
       *  Получить данные mqtt, сравнить, при изменении - обновить
       *  
       *  Установить режим вентилятора
       *  TODO: Написать логику обработки команд и показаний датчика 
       */
      if(client.connected()) {
        

        /** обновить статус в mqtt
         * TODO: Параметры - температура,влажность,времы включения и выключения, задержки итп.
         */ 
        client.publish(FAN_SPEED, (digitalRead(RELAY_PIN) != 0) ?  "0" : "100");
        client.subscribe(FAN_SPEED_CMD);
        client.publish(FAN_SPEED, (digitalRead(RELAY_PIN) != 0) ?  "0" : "100");
        Serial.println();
        String output;
        //serializeJsonPretty(initConfig, output);

        serializeJson(initConfig, output);
        #ifdef DEBUG
        Serial.println(output);
        #endif
        client.publish(CONFIG, output.c_str());
        client.subscribe(CONFIG_CMD);

        // Получение данных с датчика
        float t = sensorTemperature;
        float h = sensorHumidity;
        int err = SimpleDHTErrSuccess;
        if ((err = sensor.read2(&sensorTemperature, &sensorHumidity, NULL)) != SimpleDHTErrSuccess) {
          #ifdef VERBOSE
          Serial.println("Нет данных с датчика. Система работает в слепом режиме.");
          #endif
          return;
        }
        #ifdef DEBUG
        Serial.println("Данные с датчика получены. Система в полнофункциональном режиме.");
        #endif
        sensorAvailable = true;
        // Опубликовать данные в случае их изменения
        if(sensorTemperature != t || sensorHumidity != h){
          client.publish(TEMPERATURE, String(sensorTemperature).c_str(), true);
          client.publish(HUMIDITY, String(sensorHumidity).c_str(), true);
        }
        
      }
    }   
  }

  // Утилитарные задачи
  if(!client.connected()) {
    reconnect();
  } else {
    client.loop();
  }
}