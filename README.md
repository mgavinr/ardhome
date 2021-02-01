# ardhome

1. [Partlist Instructions](docs/PARTLIST.md)
2. [Project Instructions](docs/PROJECTLIST.md)
3. [Components Instructions](docs/COMPONENTS.md)

# other links

1. https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet

NUMBERS
============
mili is 1/1000  1mA = 0.001 A
k    is 1000    1kO = 1000. O

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

Memory
=======
PROGMEM stores in PROGMEM ahahaha ha thanks, Please note that variables must be either globally defined, OR defined with the static keyword, in order to work with PROGMEM.

You can use this for strings to serial anyway .. Serial.print(F("Write something on the Serial Monitor that is stored in FLASH"));

You can use this for anything that does not change..

Here we go ..

# PROGMEM

Flash memory is used to store your program image and any initialized data. You can execute program code from flash,but you can't modify data in flash memory from your executing code. To modify the data, it must first be copied intoSRAMFlash memory is the same technology used for thumb-drives and SD cards. It is non-volatile, so your program will stillbe there when the system is powered off.

# SRAM

stack vars, global vars, static vars live here.

# EEPROM

It's a bit slower than SRAM 1 byte at a time.  You can use apis for it.  I guess handy to store stuff between restarts?

# LIMITS

```
Flash, SRAM, EEPROM
Uno:  32k, 2k, 1k  
Mega: 256k, 8k, 4k
```

```
0x0..0xFF = 0 to 256
0xFF = 1111 1111 = 1 byte
1024 bytes = 1k

boolean       0xFF = 1 byte = true(1) or false(0) 
char          0xFF = 1 byte = -128 .. 127
unsgined char 0xFF = 1 byte = 0 .. 255
byte          0xFF = 1 byte = 0 .. 255
uint8_t       0xFF = 1 byte = 0 .. 255

int           0xFFFF = 2 byte = +-32,000
short         0xFFFF = 2 byte = +-32,000
unsigned int  0xFFFF = 2 byte = 0 .. 65000
word          0xFFFF = 2 byte = 0 .. 65000
uint16_t      0xFFFF = 2 byte = 0 .. 65000

long          0xFFFF-FFFF = 4 byte = +-2,000,000
unsigned long 0xFFFF-FFFF = 4 byte = 0 .. 4,000,000
uint32_t      0xFFFF-FFFF = 4 byte = 0 .. 4,000,000
float         0xFFFF-FFFF = 4 byte = +- something oh dear
double        0xFFFF-FFFF = 4 byte = +- something oh dear
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

IR protocol
============

Here is an example output
```
LEN =    11
START_H: 69
START_L: 34
DATA_H:   9
DATA_L:  25
DATA_LEN = 6
DATA: 0x40 0x04 0x05 0x38 0x48 0x75 
DATA:   64    4    5   56   72  117

LEN =    0000 1011
START_H: 0100 0101
START_L: 0010 0010
DATA_H:  0000 1001 
DATA_L:  0001 1001
DATA_LEN = 0000 0110
DATA: 0x40 0x04 0x05 0x38 0x48 0x75 
DATA:   64    4    5   56   72  117 
```

I don't know what the START_H START_L DATA_H DATA_L is, but DATA_LEN and DATA makes sense
I will guess:
If len is 6 => 2 tag device, and 4 tag code
If len is 4 => 2 tag device, and 2 tag code
Some IRs send the same code twice.
So you could probably ignore that.

Some more info:

start signal high time / start high/long duration
start signal low time / start low/short duration
bit high time / logic long duration
bit low time / logic short duration

whatever they are it seems they are needed, strange, anyway you don't need to send the msg twice, phew

BATTERIES
============

* If your circuit draws 35mA, a battery with a 3400mAh rating can be expected to last about 4 days.
explain?  maybe later you should know what i mean
* If you connect 4 1.5v AAs 2000mA batteries along the + line, that is believe it or not a 1.5v battery still.  But it can send something made up on a website
* AA battery 1.5v and up to 50mA.  But they write it 1500mAh - mili amp hour?  Which means?  I can be exact, it means it can supply 1500mA in one hour.  That is great.
A typical arduino uses 80mA - that means no go.  Ok a typical Arduino uses 25mA lets say.  So those batteries can power it.  That means a 1.5v stepped up can power it for 60 hours by my reconning.  And 60 hours is what exactly?  2.5 days. 
* You can connect two power packs in parallel to create more current (double so called), but with the same voltage.

Here is another example, this time from the web:
* A 12V 2400mAh - circuit 12V 300mA => 2400/300 = 8 hours

Someone suggests wiring a series diode to the 5v battery in ..

Someone suggests using the barrel and it's regulator is wasting battery and heat and stuff.. and resources to climb above the 6V.

Someone suggests 6 AAs into the barrel is the way to go as that's what it's for.  Other above is to fry stuff if you ..

Someone suggests using less batteries 3 and a DC booster.  You loose the least then.  I believe that is direct in no regs.

Someone suggests 3 AAs direct to vin, that way no power wasted on reg, and you get 6 months..

If you use 12V on the reg, you are throwing away half the power as heat.

Some interesto stuff there.  But me being me wants to have a battery backup supply.

Ah very interesting, most USB power banks require a minimum load or they shut off, therefore, you need to periodically pulse a high power LED.  soooo maybe it's best to go battery only? instead of the power bank.  Here is an example: This particular arrangement works for my XiaoMi PowerBank (20A).  Total power drawn is roughly 3.5mAh (per hour), so its not too bad.  Assuming it runs at 5v (from the USB)... 0.0035Ah x 5v = 0.0175Wh If you run it for 24 hours.. 0.0175Wh x 24 = 0.42Wh Assuming the powerbank only holds 15A (x3.7v = 55Wh) ...  this circuit will finish the battery in about 4 months.


# Arduino inputs

The ard has 4 ways to power it.  
* USB is 5V in or else.  There is a current limit of 250mA or 500mA depending on moon phase.
* The barrel jack is +7 to 12v in, and the positive is the central part of the jack  Maybe min 6V would be enough.
* Vin as above, but can be used in conjunction to the above if you need to split, but in general don't use both
* The 5V pin, it is for output normally, but you can input it if you know it is 5V and nothing but

Interestingly the ard has a comparison circuit.  So if the USB and barrel is powered, the power comes from the barrel first, and then switches over to the usb.  Why? Why not.  I would like the reverse.

There is a P MOSFET so I just need a P MOSFET and switch around the inputs right?  Anyway the P MOSFET has:
```
               5V USB
                 |
                 |
   barrel ----[FDN340P]
                 |
                 +--- 5V in to ard
```
In the above case the barrel is also connected to an opamp? comaritor that compares the input to 3V3 for something else.

I guess you could keep the barrel and then instead of USB powered the USB 5v could be applied directly to the Vin pin - that way you would get your order or precedence, but the Vin line is not protected at all..

There is also the 5V line, but you normally draw there instead of power there.

In fact that don't mention applying there and the barrel just one or the other.

There are some good ways to destroy the ards here.

The ard doesn't require 5v per se, but the Atmel chip needs 1.8V to 5.5V.  Anything below 4V will mean the 3.3V regulator will not work, and also the the Atmel chip will not work reliably at high clock speeds, so some 3.3V ards use 8MHz instead of 16MHz.

# Arduine sleep mode

Use PWM to reduce power draw.  Here are some power saving notes for the chip
http://www.gammon.com.au/power




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
