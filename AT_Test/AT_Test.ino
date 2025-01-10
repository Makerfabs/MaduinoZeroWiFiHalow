/*
Arduino IDE v1.8.19
Board: arduino_zero_native v1.8.2
 */
#include <Arduino.h>

#define SerialMon SerialUSB
#define SerialTX_AH Serial1

void setup(void)
{
  SerialMon.begin(115200);
  SerialTX_AH.begin(115200);
  delay(2000);
  SerialMon.println("\nMaduino Zero WIFI Halow AT Test Begin!\r\n");
  delay(500);
}

void loop(void)
{
  while (SerialTX_AH.available())
  {
    SerialMon.write(SerialTX_AH.read());
    yield();
  }
  while (SerialMon.available())
  {
    SerialTX_AH.write(SerialMon.read());
    yield();
  }
}
