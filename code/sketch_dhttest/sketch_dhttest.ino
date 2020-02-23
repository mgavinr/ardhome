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
#include <OneWire.h>
#include <IRSendRev.h>

#define COLORED     0
#define UNCOLORED   1

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

/*
 * Arduino uno: pin 2 temp, pin A4 A5 led, pin 4 water temp, pin 13 11 10 9 8 7 paper 
 * and then all rest are 5/3/0V it maybe an idea to glue on a header with loads of space for 5v and 0v
 * instead of filling up a breadboard.
 */
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

class myinoutIR {
    const byte _pin;

  public:
    myinoutIR(byte attachTo) :
      _pin(attachTo)
    {
    }

    ~myinoutIR()
    {
    }

    void setup() {
      Serial.print("IR read ");Serial.println(_pin);
      IR.Init(_pin);
    } 
    void loop(unsigned long milliseconds) {
      unsigned long beg_milliseconds = millis();
      unsigned long cur_milliseconds = millis();
      unsigned char dta[20];

      Serial.println("Checking for IR messages:");
      while((cur_milliseconds - beg_milliseconds) < milliseconds) {
        cur_milliseconds = millis();
        unsigned long ir_device = 0;
        unsigned long ir_button = 0;
        unsigned long ir_group = 0;
        
        if (IR.IsDta()) {               // get IR data
            // TODO some devices send the same code twice
            IR.Recv(dta);               // receive data to dta
            
            if (dta[BIT_DATA_LEN] == 4) {
              ir_device = (dta[BIT_DATA+1] + (dta[BIT_DATA+0] << 8)) & (0x0000FFFF);
              ir_button = (dta[BIT_DATA+3] + (dta[BIT_DATA+2] << 8)) & (0x0000FFFF);
            } else if (dta[BIT_DATA_LEN] == 6) {
              ir_device = (dta[BIT_DATA+1] + (dta[BIT_DATA+0] << 8)) & (0x0000FFFF);
              ir_group  = (dta[BIT_DATA+3] + (dta[BIT_DATA+2] << 8)) & (0x0000FFFF);
              ir_button = (dta[BIT_DATA+5] + (dta[BIT_DATA+4] << 8)) & (0x0000FFFF);
            }
            Serial.print("IR device=0x"); Serial.print(ir_device, HEX);
            Serial.print(" IR group="); Serial.print(ir_group, HEX);
            Serial.print(" IR button=0x"); Serial.println((unsigned long)ir_button, HEX);
            debug(dta);
        } 
      }
      Serial.println("Finished Checking for IR messages");
    }

    void debug(unsigned char* dta) {
        Serial.println("+------------------------------------------------------+");
        Serial.print("LEN = ");
        Serial.println(dta[BIT_LEN]);
        Serial.print("START_H: ");
        Serial.print(dta[BIT_START_H]);
        Serial.print("\tSTART_L: ");
        Serial.println(dta[BIT_START_L]);

        Serial.print("DATA_H: ");
        Serial.print(dta[BIT_DATA_H]);
        Serial.print("\tDATA_L: ");
        Serial.println(dta[BIT_DATA_L]);

        Serial.print("\r\nDATA_LEN = ");
        Serial.println(dta[BIT_DATA_LEN]);

        Serial.print("DATA: ");
        for (int i = 0; i < dta[BIT_DATA_LEN]; i++) {
            Serial.print("0x");
            Serial.print(dta[i + BIT_DATA], HEX);
            Serial.print("\t");
        }
        Serial.println();

        Serial.print("DATA: ");
        for (int i = 0; i < dta[BIT_DATA_LEN]; i++) {
            Serial.print(dta[i + BIT_DATA], DEC);
            Serial.print("\t");
        }
        Serial.println();
        Serial.println("+------------------------------------------------------+\r\n\r\n");
    }

    void send() {
      unsigned char dtaSend[20];
      dtaSend[BIT_LEN]        = 11;     // all data that needs to be sent
      dtaSend[BIT_START_H]    = 179;    // the logic high duration of "Start"
      dtaSend[BIT_START_L]    = 90;     // the logic low duration of "Start"
      dtaSend[BIT_DATA_H]     = 11;     // the logic "long" duration in the communication
      dtaSend[BIT_DATA_L]     = 33;     // the logic "short" duration in the communication

      dtaSend[BIT_DATA_LEN]   = 6;      // Number of data which will sent. 
      //If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

      //0x40 0x04 0x05 0x38 0x48 0x75
      dtaSend[BIT_DATA + 0]   = 0x40;      // data that will sent
      dtaSend[BIT_DATA + 1]   = 0x04;
      dtaSend[BIT_DATA + 2]   = 0x05;
      dtaSend[BIT_DATA + 3]   = 0x38;
      dtaSend[BIT_DATA + 4]   = 0x48;
      dtaSend[BIT_DATA + 5]   = 0x75;
      IR.Send(dtaSend, 38);
      Serial.println("IR Sent the data");
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

    void setup(boolean run) {
      if (run == false) return;
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

// todo convert to any number of sensors not 2
class myinWatertemp {
    byte _pin;
    byte _type_s;
    byte _init;
    OneWire* _osensor;
    byte _addr[2][8]; // two sensors and addresses
    float _temperature[2];

  public:
    myinWatertemp(byte attachTo) :
      _pin(attachTo),
      _type_s(0),
      _init(0),
      _osensor(nullptr)
    {
      _osensor = new OneWire(_pin);
    }

    ~myinWatertemp()
    {
      delete _osensor;
    }

    // TODO it's possible this may use the same addr for both arrays[2], but that is not often
    void setup() {
      byte i;
      byte max = 10;

      while(_init < 2) {
        _temperature[_init] = 0;
        if ( !_osensor->search(_addr[_init])) {
          _osensor->reset_search();
          delay(250);
        } else {
          Serial.print(_init);
          Serial.print(" ROM =");
          for( i = 0; i < 8; i++) {
            Serial.write(' ');
            Serial.print(_addr[_init][i], HEX);
          }

          if (OneWire::crc8(_addr[_init], 7) != _addr[_init][7]) {
              Serial.println(" CRC is not valid check your wiring!");
              return;
          }
          Serial.println();
       
          // the first ROM byte indicates which chip
          switch (_addr[_init][0]) {
            case 0x10:
              Serial.println("  Chip = DS18S20");  // or old DS1820
              _type_s = 1;
              break;
            case 0x28:
              Serial.println("  Chip = DS18B20");
              _type_s = 0;
              break;
            case 0x22:
              Serial.println("  Chip = DS1822");
              _type_s = 0;
              break;
            default:
              Serial.println("Device is not a DS18x20 family device.");
              return;
          } 

          _osensor->reset();
          _osensor->select(_addr[_init]);
          _osensor->write(0x44, 1);        // start conversion, with parasite power on at the end
        }
        delay(500);     // maybe 750ms is enough, maybe not
        ++_init;
        --max;
        if(max < 0) break;
      }
    }

    void loop() {
      byte data[12];
      byte i;
      byte c;
      byte present = 0;
      float celsius, fahrenheit;

      if (_init <= 0) return;

      for(c=0; c<_init; c++) {
        present = _osensor->reset();
        _osensor->select(_addr[c]);    
        _osensor->write(0xBE);         // Read Scratchpad

        Serial.print(c);
        Serial.print(" Data = ");
        Serial.print(present, HEX);
        Serial.print(" ");
        for ( i = 0; i < 9; i++) {           // we need 9 bytes
          data[i] = _osensor->read();
          Serial.print(data[i], HEX);
          Serial.print(" ");
        }
        Serial.print(" CRC=");
        Serial.print(OneWire::crc8(data, 8), HEX);
        Serial.println();

        // Convert the data to actual temperature
        // because the result is a 16 bit signed integer, it should
        // be stored to an "int16_t" type, which is always 16 bits
        // even when compiled on a 32 bit processor.
        int16_t raw = (data[1] << 8) | data[0];
        if (_type_s) {
          raw = raw << 3; // 9 bit resolution default
          if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
          }
        } else {
          byte cfg = (data[4] & 0x60);
          // at lower res, the low bits are undefined, so let's zero them
          if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
          else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
          else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
          //// default is 12 bit resolution, 750 ms conversion time
        }
        celsius = (float)raw / 16.0;
        fahrenheit = celsius * 1.8 + 32.0;
        
        Serial.print("  Temperature = ");
        Serial.print(celsius);
        Serial.print(" Celsius, ");
        Serial.print(fahrenheit);
        Serial.println(" Fahrenheit");
        _temperature[c] = celsius;
      }
    }

    void debug() {
    }
};
/*---------------------------------------
* My vars
*---------------------------------------*/
myinDTemp dtemp(2);
myoutPaper paper;
myinWatertemp watertemp(4);
myinoutIR irin(6);

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  dtemp.setup();
  paper.setup(false); // does everything, false => skip
  watertemp.setup();
  irin.setup();
}

void loop() {
  dtemp.loop();
  paper.loop(); // nothing
  watertemp.loop();
  irin.loop(50000);
  delay(5000); //Delay 2 sec. 
}
