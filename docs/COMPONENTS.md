# components

Insulators
============
* Some insulators allow heat thru, and don't conduct, for attaching heat sinks
* The Drain is sometimes the heat sink attachment

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
* 1uF = 0.000001F
* 1mA = 0.001A
* High input impedance means it draws little current

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
P = IIR
watts = amps * voltage
miliwatts = miliamps * voltage

For a transistor the voltage is the voltage dropped across the collector-emitter which is usually 0.2v.  An example for 200mA of current, that is 40 miliwatts of power.

For a mosfet the R is RDSon and the current is whatever your circuit load will draw.  For example a moter 1A, that is 35miliohm is 35miliwatts.

P dissaption = (max temp - ambient) / (per watt coeff) 
P dissaption = 175 - 25 / 62.5 = 2.4 watts
```

Transisters
============
* On off switch, or amplify small small small signals in the active region.
* BJT is used to switch motors, as they gobble up current?
* MOSFET is used for power circuits, as they grow moss.
* The metal ones are good for heat dissapation, the thru hole is for a heatsink you glue on
* Notes
- Depletion mode acts like a closed switch top to bottom if no voltage diff between gate and source.  But if you reverse the voltage like wire it wrong, it is open switch.  This seems kinda like a diode to me.  Depletion is drawn with no dashes.
- Enhancement mode acts like a variable resister.  The symbol is an E with arrow in middle dash.  The base is a L kicked over.  Base=Gate Emiiter=Source Collector=Drain. D ---> S
- When voltage is applied between gate and source, high side DS current is on.  When a low voltage is applied, the DS resistence is high.  When some voltage the resistenace is low very low, it is RDSon and 27 mili ohm.  A logical level mosfet has a VGS|VTH value of < 5v, that is, you can turn it on (resistence near zero) with less than 5v drop at V gate-source.  When you apply a greater than VGS value here, no current really flows through from that side of the transister, it's just voltage.

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

## MOSFET NCHANNEL

```
               VCC
                |
             [MOTOR] flyback allow up, block down
                |
                | Drain
              |-+
    Gate -+---|<+
          |   |-+
          |     | Source
          R     |
          |     |
          _     _

```

## MOSFET PCHANNEL

```
               VCC
                |
                |
                |
              |-+ Source
    Gate -+---|->           
          |   |-+           
          |     | Drain
          R     |
          |  [MOTOR] flyback allow up, block down
          |     |
          _     _

Not sure about this one, but if you have 0 voltage here at gate it is on.
```


Diodes
============

* Control the direction of current through circuit
* schottky drop very little voltage, like 0.2v otherwise the same.  Their symbol is right angle S
* a zener diode is designed to be wired the wrong way, and has a reference voltage of x when it reaches a certain breakdown voltage.  Their symbol is a slanted Z.  You can't abuse them by over doing the voltage, check the power across them based on their reference voltage drop.  They are written 5v1 or 3v3.
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

## max, and clippers 

I have no idea on this afraid.

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
