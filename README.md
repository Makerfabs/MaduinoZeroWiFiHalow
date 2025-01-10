# MaduinoZeroWiFiHalow

```c++
/*
Version:	    V1.0
Author:		    Yuki
Create Date:	2025/1/10
Note:
*/
```

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

## Intruduce

Product Link : [Maduino Zero WiFi Halow]()

Wiki Link : [Maduino Zero WiFi Halow](https://wiki.makerfabs.com/Maduino%20Zero%20WiFi%20Halow.html)


## Example

### AT command Test

This demo program allows users to send AT commands to the module through the serial communication, and is designed to provide a convenient way for users to test the module, query the current status of the module, and other related operations.

### Transmits T&H data to another board(wifi_halow_STA and wifi_halow_AP)

This demo sets one board to AP mode, which is responsible for receiving temperature and humidity data; And the other board is set to STA mode, which connects to the DHT11 for collecting temperature and humidity data and sending the data to the AP.

## Environment

Arduino IDE v1.8.19
Board: arduino_zero_native v1.8.2
Library: DHT sensor library v1.4.6