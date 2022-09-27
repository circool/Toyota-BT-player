
// // #include <EEPROM.h>       //https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
// // #define EEPROM_SIZE 12
// /* OTA */
// // #include <ESP8266mDNS.h>
// // #include <WiFiUdp.h>
// // #include <ArduinoOTA.h>

// /* wifi */
// // #define essid               "iot"
// // #define wifiPassword        "49414d14r2adu"

// /* mqtt */
// // #define mqttServer              "192.168.100.223"
// /* Управляющие топики  */
// // #define commandTopic                      "fans/fan1/commands"
// /* Статусные топики */
// // #define stateTopic                        "fans/fan1/states"


// // #define humidityStateTopic                "fans/fan1/states/humidity"
// // #define temperatureStateTopic             "fans/fan1/states/temperature"

// // #define powerStateTopic                       "fans/fan1/states/power"
// // #define powerCommandTopic                     "fans/fan1/commands/power"

// // #define humidityLevelOnStateTopic             "fans/fan1/states/humidity_level_on"
// // #define humidityLevelOnCommandTopic           "fans/fan1/commands/humidity_level_on"
// // #define humidityLevelOffStateTopic            "fans/fan1/states/humidity_level_off"
// // #define humidityLevelOffCommandTopic          "fans/fan1/commands/humidity_level_off"
// // #define humidityLevelControlStateTopic        "fans/fan1/states/humidity_control"
// // #define humidityLevelControlCommandTopic      "fans/fan1/commands/humidity_control"

// // #define temperatureLevelOnStateTopic          "fans/fan1/states/temperature_level_on"
// // #define temperatureLevelOnCommandTopic        "fans/fan1/commands/temperature_level_on"
// // #define temperatureLevelOffStateTopic         "fans/fan1/states/temperature_level_off"
// // #define temperatureLevelOffCommandTopic       "fans/fan1/commands/temperature_level_off"
// // #define temperatureLevelControlCommandTopic   "fans/fan1/commands/temperature_control"

// // #define timerOnStateTopic                     "fans/fan1/states/timer_on"
// // #define timerOnCommandTopic                   "fans/fan1/commands/timer_on"
// // #define temperatureLevelOffStateTopic         "fans/fan1/states/temperature_level_off"
// // #define temperatureLevelOffCommandTopic       "fans/fan1/commands/temperature_level_off"

// /* К какому пину подключен датчик */
// // #define SENSOR_PIN 1


// /* Глобальные переменные */


// /** Устанавливает состояние вентилятора
//  * TODO: Записать состояние в mqtt
//  * @param state истина - вкл; ложь - выкл
//  */ 
// void powerRelay(bool state){
//   if( state != digitalRead(RELAY_PIN) ) {
//     digitalWrite(RELAY_PIN, state);
//     Serial.printf("Реле установлено в состояние %d.\n",digitalRead(RELAY_PIN));
//   }
//   return;
// }

// /** Эта процедура вызывается при получении даных из подписанного канала MQTT
//  * @param topic mqtt-топик
//  * @param payload содержимое команды
//  * @param lenght длина команды ???
//  */
// void callback(char* topic, byte* payload, unsigned int length) {
//   // Преобразовать топик в строку
//   String recievedPayload;
//   for (unsigned int i = 0; i < length; i++) {
//     recievedPayload += (char)payload[i];
//   }

//   // Определить назначение команды и выполнить соответствующее действие
//   if (recievedPayload == "ON") {
//     digitalWrite(RELAY_PIN, LOW);
//     //relayState = true;
//     client.publish("fans/fan1/states/power", "ON");
//   } 
//   if (recievedPayload == "OFF") {     
//       digitalWrite(RELAY_PIN, HIGH);
//       //relayState = false;
//       client.publish("fans/fan1/states/power", "OFF");
//   } 
// }
// String macToStr(const uint8_t* mac) {
// // Функция для получения мак-адреса из массива char
//   String result;
//   for (int i = 0; i < 6; ++i) {
//     result += String(mac[i], 16);
//     if (i < 5)
//       result += ':';
//   }
//   return result;
// }
// void reconnect() {
// // Эта процедура вызывается при потере соединения с брокером MQTT
//   if (!client.connected()) {     
//     // Создание идентификатора client ID в формате 'esp8266-<macaddress>-<8бит_счетчика_микросекунд>
//     String clientId = "esp8266-";
//     uint8_t mac[6];
//     WiFi.macAddress(mac);
//     clientId += macToStr(mac);
//     clientId += "-";
//     clientId += String(micros() & 0xff, 16);
    
//     Serial.print("Попытка подключиться к mqtt брокеру по адресу "); 
//     Serial.print(MQTT_SERVER); 
//     Serial.print(" с идентификатором "); 
//     Serial.print(clientId); 
//     Serial.println(".");
    
//     if (client.connect(clientId.c_str())) {
//       Serial.println("Подключено.");     
//       // Опубликовать статус реле
//       if (digitalRead(RELAY_PIN) == 1) {      
//         client.publish("fans/fan1/states/power", "ON"); 
//       } else  {    
//         client.publish("fans/fan1/states/power", "OFF"); 
//       }
//       client.subscribe(MQTT_CMD);              // Подписаться на получение команд          
//     } else {
//       Serial.print("Подключение не установлено ( rc="); Serial.print(client.state()); Serial.println(").");
//       Serial.println("Повторная попытка через 5 секунд");    
//       delay(5000);
//     }
//   }
// }




// /* Доступность датчика */
// // bool sensorValid = true;

// /* Доступность wifi */
// // bool wifiValid = true;

// /* Состояние реле (вкл/выкл) */
// // bool powerState = false;

// /* Текущая температура */
// // float temperatureState = 0;

// /* Текущая влажность */
// // float humidityState = 0;

// /* Температура включения реле (Cº) */
// // int tempOn = 27;
// /* Температура выключения реле (Cº) */
// // int tempOff = 20;
// /* Использовать контроль температуры */
// // bool humidityLevelControl = false;

// /* Уровень влажности (%) при котором включать вентилятор  */
// // int humidityLevelOn = 35;
// /* Уровень влажности (%) при котором выключать вентилятор  */
// // int humidityLevelOff = 20;

// // ESP8266WiFiMulti wifiMulti;
// // WiFiClient espClient;
// // PubSubClient client(espClient);
// // SimpleDHT22 dht22(sensorPin);

// /* Задержка времени перед включением реле (мсек)  */
// // int timeOn = 10000;
// /* Задержка времени перед выключением реле (мсек)  */
// // int timeOff = 3000;
// /* Использовать контроль времени */
// // bool timeUsing = true;



// /* Эта процедура вызывается при получении даных из подписанного канала MQTT 
//  @param topic - топик, в который нужно публиковать
//  @param payload - текст топика
//  @param length - длина текста
// */
// void callback(char* topic, byte* payload, unsigned int length) {
//   String recievedPayload;
//   for (unsigned int i = 0; i < length; i++) {
//     recievedPayload += (char)payload[i];
//   }


//   // char a = powerStateTopic;
//   // if(topic == a){
//   //   // ....
//   // }


  
// //   if (recievedPayload == "1" || recievedPayload == "ON") {
// //     digitalWrite(relayPin, LOW);
// //     powerState = true;
// //     client.publish(powerStateTopic, "ON");
// //   } 
// // if (recievedPayload == "0" || recievedPayload == "OFF") {     
// //     digitalWrite(relayPin, HIGH);
// //     powerState = false;
// //     client.publish(powerStateTopic, "OFF");
// //   } 
// // }

// // /* Преобразует булево значение в строки "ON" или "OFF" */
// // char convertState(bool state){
// //   return 'c';
// // }

// //Init EEPROM
//  	EEPROM.begin(EEPROM_SIZE);
//  	//Write data into eeprom
//  	int address = 0;
//  	int boardId = 18;
//  	EEPROM.put(address, boardId);
//  	address += sizeof(boardId); //update address value
//  	float param = 26.5;
//  	EEPROM.put(address, param);
//  	EEPROM.commit();
//  	//Read data from eeprom
//  	address = 0;
//  	int readId;
//  	EEPROM.get(address, readId);
//  	Serial.print("Read Id = ");
//  	Serial.println(readId);
//  	address += sizeof(readId); //update address value
//  	float readParam;
//  	EEPROM.get(address, readParam); //readParam=EEPROM.readFloat(address);
//  	Serial.print("Read param = ");
//  	Serial.println(readParam);
//  	EEPROM.end();




//   // Проверка датчика
//   Serial.println("Проверка наличия датчика ...");
//   int err = SimpleDHTErrSuccess;
//   if ((err = dht22.read2(&temperatureState, &humidityState, NULL)) != SimpleDHTErrSuccess) {
//     Serial.printf("Ошибка датчика, код=%d, (%d). Работа будет продолжена без датчика.\n", SimpleDHTErrCode(err),SimpleDHTErrDuration(err));
//     sensorValid = false;
//   } else {
//     Serial.printf("Датчик обнаружен. Темпетатура = %lf, Влажность = %lf", temperatureState, humidityState);
//     sensorValid = true;
//   }

//   // Проверка интернета
//   Serial.println("Попытка соединения с сервером ...");
//   // Подключить WiFi
//   WiFi.mode(WIFI_STA);
//   //wifiMulti.addAP(essid, wifiPassword);

//   // Проверка mqtt

//       // Публикация текущих значений датчика
//       // client.publish(temperatureStateTopic, String(temperarureState).c_str(), true);
//       // client.publish(humidityStateTopic, String(hummidityState).c_str(), true);
//       // client.publish(humidityStateTopic, String(hummidityState).c_str(), true);
//   client.setServer(mqttServer, 1883);
//   client.setCallback(callback);
//   client.subscribe(commandTopic);


//   //wifiValid = wifiMulti.run() == WL_CONNECTED;
  
//   if(wifiValid){
//     Serial.println("WiFi найден.");
//   } else {
//     Serial.println("NONE");
//   }

//   /* Отправляет текущий статус на сервер mqtt 
//   @return Булево - результат выполнения всех запросов
// */
// bool updateState() {
//   bool returnCode = true;
//   //returnCode = returnCode & client.publish(powerStateTopic, "ON");
//   // returnCode = returnCode & client.publish(temperatureStateTopic, temperatureState);
//   // returnCode = returnCode & client.publish(humidityStateTopic, humidityState);
//   return returnCode;
// }

// void loop() {
//   //wifiValid = wifiMulti.run() == WL_CONNECTED;
//   // if(!wifiValid){
//   //   Serial.println("Нет wifi соединения.");
    

//   // }





//   // Если пришло время проверить датчик
//   // if(millis() > lastUpdate + checkPeriod) {
//   //   int err = SimpleDHTErrSuccess;
//   //   if ((err = dht22.read2(&temperatureState, &humidityState, NULL)) != SimpleDHTErrSuccess) {
//   //     Serial.print("Sensor read failed, err="); 
//   //     Serial.print(SimpleDHTErrCode(err));
//   //     Serial.print(","); 
//   //     Serial.println(SimpleDHTErrDuration(err)); 
//   //     delay(2000);
//   //     return;
//   //   } else {
//   //     //updateState();

//   //     // Публикация состояния реле
//   //     if(powerState) {
//   //       client.publish(powerStateTopic, "ON", true);
//   //       } 
//   //     else {
//   //       client.publish(powerStateTopic, "OFF", true);
//   //     }
//   //   }
//   // }
// }