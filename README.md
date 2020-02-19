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
Analog Pins 4 5
- I2C 4=SDA 5=SCL
- I2C XTR=SDA XTR-1=SCL = 18=SDA 19=SCL  .. maybe they are duplicates for handyness .. seems for shields

# PWM

```
pinMode(led_pin, OUTPUT);
analogWrite(led_pin, 0) means a signal of 0% duty cycle. OFF
analogWrite(led_pin, 127) means a signal of 50% duty cycle. 50/50
analogWrite(led_pin, 255) means a signal of 100% duty cycle. ON
```
IIC I2C
=======
SDA = serial data
SCL = serial clock 
i will never remember these accrs

```
          0v        0v
          |         |        small
ARD =========================/\/\/===5V
 |
 0v maybe the rest just reads? maybe 0v is for bus masters.
```

The I2C bus relies on open drain technology. Except for one special case in conjunction with the high speed mode no I2C device is allowed to drive the bus, i.e. to send a 1 by putting voltage on the bus. Instead, the bus is terminated to a high level, e.g. 5 V and both lines, SCL and SDA, remain there during idle mode. A device sends a 0 by pulling the bus to ground level. A 1 is sent by doing nothing, i.e. leaving the bus at high level. The termination of I2C lines is achieved by connecting the bus to the reference voltage of the bus over an appropriate resistor. Missing or wrong termination is the source of many field problems. As a rule of thumb, the higher the termination the better the signals. On the other hand a high termination bares a potential risk of damaging components. By the way, in I2C engineering terms a high termination means a strong pull-up to VCC which translates into a low value for the resistor.

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
