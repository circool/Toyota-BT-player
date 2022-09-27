/** Чувствительная информация */
#define WIFI_SSID       "iot"                 // wifi SSID
#define WIFI_PASSWORD   "49414d14r2adu"       // wifi пароль
#define MQTT_SERVER     "192.168.100.223"
#define MQTT_USER       "openhabian"
#define MQTT_PASSWORD   "re2392gramm"

/* Определение констант */
#define RELAY_PIN       0                     // К какому выводу подключено реле
#define SENSOR_PIN      1
#define SENSOR_TYPE     DHT22
#define WARM_TIME       3000                  // Время первоначальной раскрутки вентилятора
#define CHECK_PERIOD    2000                  // Периодичность проверки в дежурном режиме
#define FAN_DELAY       10000                  // Задержка включения вентилятора в автономном режиме (нет датчика и нет управления)
#define INIT_DELAY      3000                  // Задержка перед началом работы (чтоб успеть посмотреть сообщения терминала)
/* wifi */
#define CONNECT_TRYOUT  10                     // Количество попыток соединений с wifi

/* mqtt */
#define MQTT_CMD        "fans/fan1/commands"  // Управляющие топики
#define MQTT_STS        "fans/fan1/states"    // Статусные топики