/* Определение констант */
#define RELAY_PIN       0                     // К какому выводу подключено реле
#define SENSOR_PIN      2                     // К какому выводу подключен датчик

#ifdef DHT22
  #define SENSOR_TYPE DHT22
#else
  #define SENSOR_TYPE DHT11
#endif

#define WARM_TIME       300                   // Время первоначальной раскрутки вентилятора
#define CHECK_PERIOD    2000                  // Периодичность проверки в дежурном режиме
#define FAN_DELAY       100000                // Задержка включения вентилятора в автономном режиме (нет датчика и нет управления)
#define INIT_DELAY      2000                   // Задержка перед началом работы (чтоб успеть посмотреть сообщения терминала)

#define TEMP_CONTROL_ENABLE false
#define TEMP_CONTROL_OFF 0
#define TEMP_CONTROL_ON 0
#define HUM_CONTROL_ENABLE false
#define HUM_CONTROL_OFF 0
#define HUM_CONTROL_ON 0


/* wifi */
//#define CONNECT_TRYOUT  10                    // Количество попыток соединений с wifi

/* mqtt */
#define MQTT_CLIENT
#define MQTT_CMD        "fans/fan1/commands"  // Управляющие топики
#define MQTT_STS        "fans/fan1/states"    // Статусные топики
#define TEMPERATURE     "fans/fan1/states/temperature"
#define HUMIDITY        "fans/fan1/states/humidity"


/**
 * TODO: Предусмотреть возможность управления скоростью вращения
 */ 
#define FAN_SPEED           "fans/fan1/states/speed"
#define FAN_SPEED_CMD       "fans/fan1/commands/speed"

/**
 * TODO: 
 */ 
#define CONFIG           "fans/fan1/states/config"
#define CONFIG_CMD        "fans/fan1/commands/config"