# Toyota B[lue]T[ooth] player
Позволяет использовать внешний аудио источник как cd-chander, что делает возможным прослушивание его на штатном головном устройстве и управлять им посредством штатных средств авто.

## Resource
- [Шина передачи данных IE-Bus и AVC-LAN автомобилей Toyota](https://blog.instalator.ru/archives/1146)
- [AVC-Lan - how it work ?](http://www.softservice.com.pl/corolla/avc/avclan.php)
- [Just enable AUX input](http://www.softservice.com.pl/corolla/avc/simpleaux.php)
- [Активация AUX эмуляцией чейнджера на Arduino](https://github.com/instalator/AVC-LAN-Toyota)
- [AVC_Driver CD_changer emulator and Weel control unit are done](https://github.com/DonVas/AVC_LAN_Toyota_Corolla)

## Target
Целью проекта является создание устройства с помощью которого станет возможно прослушивание и  управление функциями bluetooth mp3-плеера посредством штатных средств управления автомобиля.
Интерфейс маскируется под внешний CD-CHANGER и поддерживает основные команды управления (в той части, где они пересекаются с командами аудиоплеера). 

## Hardware

### ESP32 - BT + CPU

Планируется реализовать блок связи блютуз и возможности ЦПУ для реализации функционала контроллера CD-CHANGER.
