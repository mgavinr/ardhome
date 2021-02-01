# Partlist Instructions

## Power

### 78XX Linear regulator

* These have an input, output and reference usually tied to ground.  
* The in and out are also right angled capacitors that are anything but this guy picks 2.2uF input and 100uF for output.

### LM317 Linear regulator

* These have an input, output and adjust.  
* It is is variable regulator, out set from 1.25 to 37v to a max 1.5A.
* Again you can have caps on the outside at right angles to ground.
* You can think of the adjust hear going to a voltage divider at the output side to ground in the middle.
* The caps filter the input/output for ..

Vout = 1.25 * (1 + RL/RH)
Vout = 1.25 * (1 + 820/220)
Vout = 4.8

### PSM-165 Module

* 3.3 v regulator from 4.5 to 12v.  Up to 800mA.  It is the one on the Arduino.  There is another one that is good for batts, loosing only about 1v.

### L4931CZ33 

* 3.3V or 5v with low v drop like 0.4v regulator.  Looks like a trans, rather than a chip.  Add caps again.

### Bucks 

* are different to linear, and good for batteries, as they are more efficient.
* all above are high to ref (low)

### Boost converter

* these are low to high
* PSM-205 is a low v to usb 5v.
* good to power motors from batteries

### Buck Boost

* Batteries start high reg down, go low reg up, can get hot.

## Chips

### Ard timer chip


## Sensors

### DollaTek DHT22/AM2302 Digital Temperature And Humidity Sensor Module Replace SHT11 SHT15 

```
Pins: 5v Data 0v
Code: Include DHT library and read the data
```
```
// vars
#include <DHT.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float hum;  //Stores humidity value
float temp; //Stores temperature value

// code
dht.begin(); // in setup
hum = dht.readHumidity();
temp= dht.readTemperature();
```

### LEDs / Segmented / Red 7-segment clock display - 0.56" digit height

This is a Common Cathode LED display

```
Pins: 
    Connect CLK to the I2C clock - on Arduino UNO thats Analog #5, on the Leonardo it's Digital #3, on the Mega it's digital #21
    Connect DAT to the I2C data - on Arduino UNO thats Analog #4, on the Leonardo it's Digital #2, on the Mega it's digital #20
    Connect GND to common ground
    Connect VCC+ to power - 5V is best but 3V also seems to work for 3V microcontrollers.
```
```
// vars
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

//code
matrix.begin(0x70);
matrix.print(10000, DEC);
matrix.writeDisplay();
delay(500);
matrix.print(0xBEEF, HEX);
matrix.writeDisplay();
delay(500);
```

You can also use the other class, alpha though its a bit strange for numbers, i might merge the 2 yes? no

### waveshare 2.13inch e-Paper HAT (C)

2 colour red and black e-paper.

For a colour byte there is something to do with 0x80

Note: Because of the small RAM of Arduino, it cannot support drawing function, therefore, we only provide image display function. The image data are stored in flash. 

It is not the V2 one 

blue pins summary: swapped for added brilliance: orange black, 13, 11 10 9 8 7

```
Pins

Vcc 	 5V or 3.3v preferably?
GND 	GND
DIN 	D11
CLK 	D13
CS 	  D10
DC 	   D9
RST 	 D8
BUSY 	 D7 
```

```
Code
https://github.com/waveshare/e-Paper
You need to create the library directory manually by copying over only the required files

#include <SPI.h>
#include "epd2in13b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

// setup
      Epd _epd;

      if (_epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
      }
      _init = true;
// draw ... hmmm lots to it see below
```

```
To draw you need to use windows to update the full memory of the device as ard has very little memory for 1 full global array of picture data .. 

#define EPD_WIDTH       104 (coords)
#define EPD_HEIGHT      212 (coords)

// creating dots and fonts
Paint(unsigned char* image, int width(coords), int height(coords));
  void DrawAbsolutePixel(int x, int y, int colored);
  void DrawPixel(int x, int y, int colored);
  void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored); // words
  void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored); // words
  void DrawLine(int x0, int y0, int x1, int y1, int colored);
  void DrawHorizontalLine(int x, int y, int width, int colored);
  void DrawVerticalLine(int x, int y, int height, int colored);
  void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
  void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
  void DrawCircle(int x, int y, int radius, int colored);
  void DrawFilledCircle(int x, int y, int radius, int colored);

/* Max size of bitmap will based on a font24 (17x24) */
#define MAX_HEIGHT_FONT         24
#define MAX_WIDTH_FONT          17
#define OFFSET_BITMAP           54

// fonts are just an array of ints I believe 8 ints high and
// 5 bits wide, 3 bits are space so e.g. 0xF8, // #####
//                                                11111000

struct sFONT {
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
};
extern sFONT Font24;
extern sFONT Font20;
extern sFONT Font16;
extern sFONT Font12;
extern sFONT Font8;

// creating images
image black and red
Is just a massive array of numbers declared:
const unsigned char IMAGE_RED[] PROGMEM = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
2756 bytes
#define EPD_WIDTH       104
#define EPD_HEIGHT      212

212*104 = 22048 dots = 2756 bytes
coords though mean:
  01..................211 : 1 line=1696 pixels, 212 hex digits
  1
  .
103...................211 : 1 row=832 pixels, 104 hex digits

// displaying
void SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l);
void SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l);
void SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l);
void DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red);
void DisplayFrame(void);
```

I'll come back to that, when i do a simulator

### Temperature Sensor - Waterproof (DS18B20) (SEN-11050) 

This is a very simple temp sensor.  Oh wait.  Good for busters..  I will figure it out one day.

An extremely detailed spec is http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Temp/DS18B20.pdf
One library is https://github.com/PaulStoffregen/OneWire.git

It has lots of code for all steps, i must write my own one.. with more comments.

### IR Receiver 38Khz - 45m range && Grove IR Emitter 10m range

```
Pins : Looking at it:  Left=Data M=+5v Right=GND
           Behind it:  Left=GND M=+5v Right=Data
Pins : Looking at it:  Left=TX NC +5v Gnd
           Behind it:  Left=GND +5v NC TX
```

```
Code
There are a few libs, this maybe the one to go for as it supports the IR send too.  
However it's not as easy to use.
I have edited the prints, and it maybe okay now
git clone https://github.com/Seeed-Studio/IRSendRev.git

// setup
#include <IRSendRev.h>
IR.Init(3);

// code
unsigned char dta[20];

// lots now ..
  if (IR.IsDta()) {               // get IR data
        IR.Recv(dta);               // receive data to dta

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

```

For emitting data I think it is fixed to pin 3, which suits me as pin 3 is free.
And all you do is call send?
There is a global object IR always avail, so you can use it, after you call init on it maybe

I am not sure one ard can do sending and receiving, we shall the see
