# Partlist Instructions

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
