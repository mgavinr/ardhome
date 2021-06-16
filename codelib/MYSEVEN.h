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
    int _select = 0;

    MYSEVEN(int select=0) : _select(select)  { }
    ~MYSEVEN() { }

    void setup() {
      int selectors[] = {0x70, 0x71, 0x72, 0x73};
#if MYSERIAL == 1
      while (!Serial);
#endif
      Serial.println("MYSEVEN.setup()");
      _matrix.begin(selectors[_select]);
      _matrix.clear();
      _matrix.setBrightness(2); // 10
      _matrix.print(selectors[_select], HEX);
      _matrix.writeDisplay();
      Serial.print("Display "); Serial.print(selectors[_select], HEX); Serial.println(" created");
    }

    void loop() {

    }

    /*---------------------------------------*/

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
      //_matrix.print(_humidity);
      _matrix.writeDisplay();
      delay(3500);

      // temp
      _matrix.clear();
      _matrix.writeDigitRaw(1, (uint8_t)0b01111000);
      _matrix.writeDigitRaw(2, (uint8_t)0b11111111);
      _matrix.writeDisplay();
      delay(1500);
      _matrix.clear();
      //_matrix.print(_temperature);
      _matrix.writeDisplay();
    }

};


/*---------------------------------------
* MYMAIN
*---------------------------------------*/
/*
vi ~/gits/ardhome/libraries/Adafruit_LED_Backpack_Library//examples/sevenseg/sevenseg.ino 
vi ~/gits/ardhome/arduino/libraries/Adafruit_LED_Backpack_Library/Adafruit_LEDBackpack.cpp 
*/
#if MYMAIN == 1
MYSEVEN myseven;

void setup() {
  Serial.begin(9600);
  myseven.setup();
}

void loop() {
  myseven.loop();
  delay(5000); 
}
#endif
