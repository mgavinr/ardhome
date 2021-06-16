/*---------------------------------------*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>

/*---------------------------------------
* classes 
*---------------------------------------*/
class MYSEVEN {
  public:
    Adafruit_7segment _matrix = Adafruit_7segment();  
    int _temperature = 8;
    int _humidity = 60;

    MYSEVEN() { }
    ~MYSEVEN() { }

    void setup() {
      _matrix.begin(0x70);
      _matrix.clear();
      _matrix.setBrightness(2); // 10
      _matrix.print(0xBEAF, HEX);

      _matrix.writeDisplay();
    }

    void loop() {

    }

    /*---------------------------------------*/
    void debug() {
      Serial.print("Temperature=");Serial.print(_temperature);
      Serial.print(" Humidity=");Serial.println(_humidity);   
    }

    void display() {
      // hum
      _matrix.clear();
                                     // 0b0001000001110000,
      // AAAAA
      // B   C
      // DDDDD
      // E   F
      // GGGGG .
      //_matrix.writeDigitRaw(1, (uint8_t)0b.BDEGFCA);
      _matrix.writeDigitRaw(1,   (uint8_t)0b01110100);
      _matrix.writeDigitRaw(2, (uint8_t)0b11111111);
      _matrix.writeDisplay();
      delay(1500);
      _matrix.clear();
      _matrix.print(_humidity);
      _matrix.writeDisplay();
      delay(3500);

      // temp
      _matrix.clear();
      _matrix.writeDigitRaw(1, (uint8_t)0b01111000);
      _matrix.writeDigitRaw(2, (uint8_t)0b11111111);
      _matrix.writeDisplay();
      delay(1500);
      _matrix.clear();
      _matrix.print(_temperature);
      _matrix.writeDisplay();
    }

};

/*---------------------------------------
* Arduino main
*---------------------------------------*/
MYSEVEN myseven;

void setup() {
  Serial.begin(9600);
  myseven.setup();
}

void loop() {
  //myseven.loop();
  myseven.debug();
  delay(5000); 
}
