/*---------------------------------------
* Includes
*---------------------------------------*/
#include <DHT.h>

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

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  button.setup();
  dtemp.setup();
}

void loop() {
  button.loop();
  dtemp.loop();
  dtemp.debug();
  delay(10000); //Delay 2 sec. 
}
