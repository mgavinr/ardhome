# Partlist Instructions

## Sensors

### DollaTek DHT22/AM2302 Digital Temperature And Humidity Sensor Module Replace SHT11 SHT15 

Pins: 5v Data 0v
Code: Include DHT library and read the data

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


