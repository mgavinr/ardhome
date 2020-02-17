# ardhome

1. [Partlist Instructions](docs/PARTLIST.md)
2. [Project Instructions](docs/PROJECTLIST.md)

# other links

1. https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet

ARDUINO
============

# Power
USB power 
=> 5v in
=> Vin is also 5v

Barrel power
=> 9-12V is Vin
=> Vin is also this

No power
=> Vin is the power in

3.3 V 
=> generated from the FTDI chip

# Pins

Digital Pins 0 1
- Serial In/Out - TX/RX (dark green) 
- These pins cannot be used for digital i/o (digitalRead and digitalWrite) if you are also using serial communication (e.g. Serial.begin). 

Digital Pins 2-13 (green) 
Digital Pins 3 5 6 10 11
- pwm
Digital Pins 10 11 12 13
- spi SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK). These pins support SPI communication, which, although provided by the underlying hardware, is not currently included in the Arduino language. 
Digial Pin 13
- when high led is also high

Analog In Pins 0-5 (light blue) 

# PWM

pinMode(led_pin, OUTPUT);
analogWrite(led_pin, 0) means a signal of 0% duty cycle. OFF
analogWrite(led_pin, 127) means a signal of 50% duty cycle. 50/50
analogWrite(led_pin, 255) means a signal of 100% duty cycle. ON

README.md 
============

```
# H1
## H2
### H3
#### H4
##### H5
###### H6

Alternatively, for H1 and H2, an underline-ish style:

Alt-H1
======

Alt-H2
------
```
