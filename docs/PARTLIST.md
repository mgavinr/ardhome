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
