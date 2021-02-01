# components

Capacitor
============
A coupling capacitor 
```
AC ----| |----------
```
If it's chosen correctly, it can block any DC offset values in AC. Choose one that allows the AC feq you want thru, e.g. 1uF is a high pass filter, that will let most freq in audio range thru.  You would also use a capacitor at the output pin, if you want to remove DC there.

Sometimes connect
```
+ ----->|----| |----- -
```

Sometimes connect
```
+ ----->|----| |----- -
          |         |
          ---\/\/\---
```

The voltage on capacitor is the max of Voltage, so if it starts at 5V but drops to 3V it is still 5V at the capacitor, for awhile anyway.  So e.g. to have a reading over a capacitor you can measure the max voltage over the last 5 seconds for a 100uF cap, but putting in a specific resister:
```
X time.discharge.seconds = 5 * RC
5  = 5RC
R = 5/(5C)
R = 5/(5*100uF)           1u = 0.000001 
R = 5/(5*0.0001uF)
R = 10000 = 5/0.000500
R = 10kohm
```

Inductor
============

* inductor resists change in current.
* Motor is like a Inductor and R together.  
* Generally you would have a diode to prevent flyback voltage, which could be -13V for a 5V fan.
```
       <- - - - fly - - high <
       
        ---------|<------         
       |                 |         
+ -----+--xxxxx--\/\/\---+---------
     - - - - normal - - - >
     high                low
```

Laws
============
* Basic
```
V = IR 
I = V/R
voltage * 1000 = miliamps * ohm
miliamps = (voltage * 1000) / ohm
```
* Power
```
P = IV
watts = amps * voltage
miliwatts = miliamps * voltage

For a transistor the voltage is the voltage dropped across the collector-emitter which is usually 0.2v.  An example for 200mA of current, that is 40 miliwatts of power.
```

Transisters
============
* On off switch, or amplify small small small signals in the active region.
* BJT is used to switch motors, as they gobble up current?
* MOSFET is used for power circuits, as they grow moss.
* The metal ones are good for heat dissapation, the thru hole is for a heatsink you glue on

Some characteristics of a BJT transisters (like 2N3904 TO-92 TO-220big)
```
some typical max voltages for wrong setup is        40 - 60v
some typical max current is                         200mA
voltage dropped from collector to emitter is        0.2v
voltage dropped from base to emitter is             0.7v
current flowing into the collector is               Beta * Ib
                                                    example: 100 * 4.3mA  (A 5V near base + 1k ohm resister means base current 4.3mA)
current flowing out of base is                      Ic + Ib

a BJT NPN (high side) is normally always off, you just need to raise the Vb to > 0.7v to turn it on.

a BJT PNP (low side) is normally always on, to turn off you need to raise the Vb so that it is the same as the VCC (within 0.7v)

so if your PSU is 5V, you raise the base to 5v, then the diode is off so transistor is off.  But if the VCC is 6V and you raise to 5V the transistor is still on.

so if your PSU is 6V and the trans is 0.2v, then the voltage drop across your circuit is 5.8v, so the 100 ohm motor current is I=V/R=5800/100=58mA and that is less than some example transistor maximums of 200mA

```

BJT: NPN 
------------
```
               VCC
                |
             [MOTOR] flyback allow up, block down
                |
                |
              |/  Collector
 VBB --[1k]---|             
              |\            
                > Emitter
                |
                |
                _

```

BJT: PNP 
------------

```
               VCC
                |
                |
                |
              |</ Emitter
 VBB --[1k]---|             
              |\            
                | Collector
                |
             [MOTOR] flyback allow up, block down
                |
                _

```

## Transister - MOSFET

    Drain
    Gate +---|<--+ 
    Source

Base controls current, top to bottom, based on voltage at Gate

Diodes
============

* Control the direction of current through circuit
* You can add a fuse to limit current too
```
---->|---
```
* They are ptype ntype, direction left to right, anode cathode (line, cathode,m is the one with the line, the blocker)
* A diode on is forward biased, V is the voltage required for it to turn on 0.7v
* off = reverse biased, Vbr is when it breaks the barrier, -100v
* An ideal diode has no voltage dropped across it
* You may need to add R to prevent the diodes from overheating in the forward bias, 1N4148 = I = 300mA is the max

## rectifiers (convert ac to dc) weiner is an example

* Uses: rectifier convert ac to dc
* For power, you may switch to a rectifier diode (! signal diode) that have higher max current rating
* dc = direct current, flows in one direction
* A full wave rectifier also converts the negative Vs
* A weiner is the 4 diode:
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
Then to smooth the above add a low pass filter before the circuit
```

## max

I have no idea.



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
