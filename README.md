# ardhome

1. [Partlist Instructions](docs/PARTLIST.md)
2. [Project Instructions](docs/PROJECTLIST.md)

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

COMPONENTS
=======

# Inductor

Motor is like a Inductor and R together
Resistence to change in current?
Flyback V, in motors and inducts?, is the V spike induced when the current changes.
-13V for a 5v fan
Diode in parallel:

       +---------|<------+         
+ -----+--xxxxx--\/\/\---+---------

# Transister
Switch or Amplify small signals.
On or off, or active region, small changes in active region in the input, means lots of changes in the output
These are active, so > power than input?
Saturation mode, fully on ..  Vce is 0.2v or Vsat .. Vbe is still 0.7v

The metal ones are good for heat dissapation, the thru hole is for a heat sink you glue on lolo

A switch? Turn on a motor with arduino?  No motor 6v and requires more current.

A 1KO resister is good for Arduino and Bases. => 4.3mA I from arduino (5.0-0.7/1000 = 4.3 mili A)

## Transister - BJT

    N Collector        HIGH     Ic            0.2v
    P Base                      Ib      Vbe = 0.7v
    N Emitter +-->+    LOW      Ib+Ic

    P Collector        LOW
    N Base
    P Emitter +<--+    HIGH

       or

    P Emitter +<--+    HIGH     Ib+Ic        -0.2v
    N Base                      Ib     Vbe = -0.7v
    P Collector        LOW      Ic

Base controls current, top to bottom, based on current at the base

Ib Cutoff mode, no current passes through, Vbe < 0.7v usually 
Otherwise any current can flow through, so you usually restister it at the base.
If the current is very small better
Ic = B * Ib  (B or hFE is the gain) say 100

e.g. 1mA into base, so the other one is 100mA
what about the other other one, it is 101mA loser
So 100mA is the Collector current

ntype or ptype
    N
    P
    N

    P
    N
    P

Vbe - voltage drop between base and emitter, Vce > 0
OMG PNP current flows into the emitter out of the base and out of the collector,
thanks be to dog.

### Transister - High/Low Switch (High/Low Side Switch)

Here is a circuit for 1 is on (NPN) as transister is low:

```

                    6V
                    +
                    |
                [ MOTOR (resister inductor + flyback diode) ]
                    |
          1k        +
  []-----\/\/\-----+|
                    ->+
                      |
                      =

```
Here is one for 0 is on (PNP) as transister is high side of circuit:
```
                    6V
                      +
                      |
                      |
          1k        <-+
  []-----\/\/\-----+|
                    |
                [ MOTOR (resister inductor + flyback diode) ]
                    |
                    =


To turn off, the Vbe 
```

## Transister - FET

    Drain
    Gate +---|<--+ 
    Source

Base controls current, top to bottom, based on voltage at Gate

# Diodes

Control the direction of current through circuit
You can add a fuse to limit current too
---->|---
ptype ntype
direction left to right
anode cathode (line, cathode,m is the one with the line, the blocker)
diode on = forward biased, Vf is the voltage required for it to turn on 0.7v
off = reverse biased, Vbr is when it breaks the barrier, -100v
ideal diode => no voltage across it
You may need to add R to prevent the diodes from overheating in the forward bias
1N4148 = If = 300mA is the max

## Diodes - uses rectifier

Uses: rectifier convert ac to dc
dc = direct current, flows in one direction
A full wave rectifier also converts the negative Vs

A weiner is the 4 diode:
```
                   +----------+
                   |          |
            +--|<--+-->|--+   |
+ ----------+             + ==|====== -
            +-->|--+--|<--+   |
                   |          |
                   |          |
               [Circuit]      |
                   |          |
                   +----------+
```

Then to smooth the above add a low pass filter before the circuit

For power, you may switch to a rectifier diode (! signal diode) that have higher max current rating

## Diodes - uses max

Connect
```
+ ----->|----| |----- -
```

The voltage on capacitor is the max of +, so if + starts at 5v goes to 3v the 
voltage across the cap is around 5v.  The cap is 100uF

You can have the max over last 5 seconds:
    Time-discharge = 5 * R * C
    5 seconds = 5 * R * C
    5/5C = R
    5/5*100uF = R
    5/0.000500 = R = 10000 = 10kO

## Diodes - clipper - earmuphs

Remove negative voltage:
```
+ ------------------- ?
       1kO
+ ----\/\/\---+------ ?
              |
- -------->|--+
```
but remember the diode, so it is -0.6v min

You can add a reverse one and remove neegatives too, so now 
you have -0.6 .. 0.6v.

You can add a psu, and you now have a range -3v .. 2v:
```
       1kO
+ ----\/\/\---+------ 2v
              |
       1.4v   |
-      +      |
- -----+-->|--+
```

But that seems like theory, unless he plans using some caps or someth, or just have a parralel circuit via the Vin?

## Diodes - shotkey sorry schottky

So normal diodes drop 0.6v these dont drop much. 0.3v
--->S--

## Diodes - zener

A zenar diode has a reference (say 3.3v value) Zener voltage when it's in breakdown mode, 
also it is placed in reverse. So their cathode faces the +PSU:

+ ----\/\/\----+
               |
               |
              _z_
              /_\  5V1 or 3V3 
               |
- -------------+

You need the resister there.  You should calculate the Power thru it based in the + - drop.
Don't bash these devices.. or you will burn, burn burn burn forever

There is something like this called a Transient Voltage some or other somewhat that drains,
usually to ground, any spikes.  The PI has one a chip near the psu..e.g.

# PSU Failover Project 

Short circuit is dangerous
So Diodes
+ ---->S----------+
+ ---->S----------+
                 [?]
- ----------------+


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
