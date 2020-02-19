/*---------------------------------------
* Includes
*---------------------------------------*/
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

/*---------------------------------------
* My objects
*---------------------------------------*/
class myinDTemp {
    const int _dht_type = DHT22;
    const byte _pin;
    float _humidity;
    float _temperature;
    DHT* _dht;
    Adafruit_7segment _matrix = Adafruit_7segment();  

  public:
    myinDTemp(byte attachTo) :
      _pin(attachTo),
      _humidity(0),
      _temperature(0),
      _dht(nullptr)
    {
    }

    ~myinDTemp()
    {
      delete _dht;
    }

    void setup() {
      _matrix.begin(0x70);
      _matrix.clear();
      _matrix.setBrightness(2); // 10
      _matrix.writeDisplay();
      pinMode(_pin, INPUT_PULLUP);
      _dht = new DHT(_pin, _dht_type);
      _dht->begin();
    }

    void loop() {
      _humidity = _dht->readHumidity();
      _temperature = _dht->readTemperature();
      display();
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

    void debug() {
      Serial.print("Temperature=");Serial.print(_temperature);
      Serial.print(" Humidity=");Serial.println(_humidity);   
    }
};

/*---------------------------------------
* My vars
*---------------------------------------*/
myinDTemp dtemp(8);

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  
  dtemp.setup();
}

void loop() {
 
  dtemp.loop();
  dtemp.debug();
  delay(5000); //Delay 2 sec. 
}
