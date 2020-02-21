/*---------------------------------------
* Includes
*---------------------------------------*/
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include "epd2in13b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

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

class myoutPaper {
  Epd _epd;
  boolean _init;

  public:
    myoutPaper() : 
      _init(false)
    {
    }

    ~myoutPaper()
    {
    }

    void setup() {
      Epd _epd;

      if (_epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
      }
      _init = true;

      /* This clears the SRAM of the e-paper display */
      _epd.ClearFrame();

      /**
        * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
        * In this case, a smaller image buffer is allocated and you have to 
        * update a partial display several times.
        * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
        */
      // max x/width/depth 104,  max y/height/length 212
      // 8y * 26 = 212
      // bytes not coords i.e. it is *8 for number of coords, 6656 pixels
      // 13 bytes is one line, 104
      // there are 

      /*
       * Title - Y axis
       */
      unsigned char image[13*26];  //1/4 image
      memset(image, 0x00, sizeof(image)); // fill solid
      Paint paint(image, 104, 26); // width,height
      paint.DrawStringAt(10, 5, "TEMPERATURE", &Font12, UNCOLORED); // indented in 8 (0-104) and down 2 (0-53)
      _epd.SetPartialWindowRed(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight()); //1
      memset(image, 0xFF, sizeof(image)); // fill empty
      paint.DrawStringAt(10, 5, "TEMPERATURE", &Font12, COLORED); // indented in 8 (0-104) and down 2 (0-53)
      _epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight()); //1

      /*
       * Data
       */
      unsigned char image2[13*26];  //1/4 image
      memset(image2, 0x00, sizeof(image2)); // fill solid
      Paint paint2(image2, 104, 26); // width,height
      //paint2.SetRotate(ROTATE_90);
      paint2.DrawStringAt(10, 5, "TIME", &Font12, UNCOLORED); // indented in 8 (0-104) and down 2 (0-53)
      _epd.SetPartialWindowRed(paint2.GetImage(), 0, 100, paint.GetWidth(), paint.GetHeight()); //1
      memset(image2, 0xFF, sizeof(image2)); // fill empty
      paint2.DrawStringAt(10, 5, "TIME", &Font12, COLORED); // indented in 8 (0-104) and down 2 (0-53)
      _epd.SetPartialWindowBlack(paint2.GetImage(), 0, 100, paint.GetWidth(), paint.GetHeight()); //1
       /* 
        * Display
        */
      _epd.DisplayFrame();
      _epd.Sleep();
    }

    void loop() {
    }

    void debug() {
      Serial.print("Paper was initialised: ");Serial.print(_init);
      Serial.println(" ");   
    }
};

/*---------------------------------------
* My vars
*---------------------------------------*/
//myinDTemp dtemp(2);
myoutPaper paper;

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  //dtemp.setup();
  paper.setup();
  paper.loop();
}

void loop() {
 
  //dtemp.loop();
  //dtemp.debug();
  //paper.loop();
  paper.debug();
  delay(5000); //Delay 2 sec. 
}
