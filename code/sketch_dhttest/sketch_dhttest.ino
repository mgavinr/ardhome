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
      pinMode(_pin, INPUT_PULLUP);
      _dht = new DHT(_pin, _dht_type);
      _dht->begin();
    }

    void loop() {
      _humidity = _dht->readHumidity();
      _temperature = _dht->readTemperature();
    }

    void debug() {
      Serial.print("Temperature=");Serial.print(_temperature);
      Serial.print(" Humidity=");Serial.println(_humidity);   
    }
};

/*---------------------------------------*/
class myoutSevenword {
    Adafruit_AlphaNum4 _matrix = Adafruit_AlphaNum4();

  public:
    myoutSevenword()
    {
    }

    ~myoutSevenword()
    {
    }

    void setup() {
      _matrix.begin(0x70);
    }

    void loop() {
      _matrix.setBrightness(0);
      _matrix.clear();
      _matrix.writeDigitRaw(0, 0xFFFF); // 8
      _matrix.writeDigitRaw(4, 0xFFFF); // 8
      _matrix.writeDisplay();
      delay(1500);
      //
      _matrix.writeDigitAscii(0, 't');
      _matrix.writeDigitAscii(1, 'E');
      _matrix.writeDigitAscii(2, ' ');
      _matrix.writeDigitAscii(3, 'M');
      _matrix.writeDigitAscii(4, 'P');
      _matrix.writeDisplay();
      delay(1500);
    }

    void debug() {
      Serial.print("7segword");
    }
};
class myoutSevennum {
    Adafruit_7segment _matrix = Adafruit_7segment();

  public:
    myoutSevennum()
    {
    }

    ~myoutSevennum()
    {
    }

    void setup() {
      _matrix.begin(0x70);
    }

    void loop() {
      _matrix.print(1000, DEC);
      _matrix.writeDisplay();
      delay(1500);
      _matrix.print(0xBEEF, HEX);
      _matrix.writeDisplay();
      delay(1500);
    }

    void debug() {
      Serial.print("7seg");
    }
};

/*---------------------------------------*/
class myinButton {
    const byte _pin;
    int _state;
    unsigned long _buttonDownMs;

  public:
    myinButton(byte attachTo) :
      _pin(attachTo)
    {
    }


    void setup() {
      pinMode(_pin, INPUT_PULLUP);
      _state = HIGH;
    }

    void loop() {
      int prevState = _state;
      _state = digitalRead(_pin);
      if (prevState == HIGH && _state == LOW) {
        _buttonDownMs = millis();
      }
      else if (prevState == LOW && _state == HIGH) {
        if (millis() - _buttonDownMs < 50) {
          // ignore this for debounce
        }
        else if (millis() - _buttonDownMs < 250) {
          // short click
        }
        else  {
          // long click
        }
      }
    }
};

/*---------------------------------------
* My vars
*---------------------------------------*/
myinButton button(7);
myinDTemp dtemp(8);
//myoutSevennum seven;
myoutSevenword sevenword;

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  button.setup();
  dtemp.setup();
  //seven.setup();
  sevenword.setup();
}

void loop() {
  button.loop();
  dtemp.loop();
  dtemp.debug();
  //seven.loop();
  //seven.debug();
  sevenword.loop();
  sevenword.debug();
  delay(1000); //Delay 2 sec. 
}
