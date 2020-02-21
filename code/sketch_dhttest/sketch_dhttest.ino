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
      unsigned char image[1024];
      Paint paint(image, 128, 18); // width should be the multiple of 8 , width, height

      paint.Clear(UNCOLORED);
      paint.Clear(COLORED);
      paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
      _epd.SetPartialWindowBlack(paint.GetImage(), 0, 8, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper1 bw");   

      paint.DrawStringAt(8, 2, "Hello world", &Font12, UNCOLORED);
      _epd.SetPartialWindowRed(paint.GetImage(), 0, 24, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper2 color");   
      
      paint.SetWidth(64);
      paint.SetHeight(64);

      paint.Clear(UNCOLORED);
      paint.DrawRectangle(0, 0, 40, 50, COLORED);
      paint.DrawLine(0, 0, 40, 50, COLORED);
      paint.DrawLine(40, 0, 0, 50, COLORED);
      _epd.SetPartialWindowBlack(paint.GetImage(), 8, 72, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper3 bw");   
      
      paint.Clear(UNCOLORED);
      paint.DrawCircle(16, 16, 15, COLORED);
      _epd.SetPartialWindowBlack(paint.GetImage(), 64, 72, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper4 bw");   

      paint.Clear(UNCOLORED);
      paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
      _epd.SetPartialWindowRed(paint.GetImage(), 8, 144, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper5 bw");   

      paint.Clear(UNCOLORED);
      paint.DrawFilledCircle(16, 16, 15, COLORED);
      _epd.SetPartialWindowRed(paint.GetImage(), 64, 144, paint.GetWidth(), paint.GetHeight());
      Serial.println("Paper6 bw");   

      /* This displays the data from the SRAM in e-Paper module */
      _epd.DisplayFrame();
      Serial.println("DISPLAY IMAGE 1");   

      /* This displays an image */
      _epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
      Serial.println("DISPLAY IMAGE 2");   

      /* Deep sleep */
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
