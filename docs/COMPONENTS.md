# components

1. Inductor(#Inductor)
2. Transister(#Transister)
2. Diodes(#Diodes)

Inductor
============

Motor is like a Inductor and R together
Resistence to change in current?
Flyback V, in motors and inducts?, is the V spike induced when the current changes.
-13V for a 5v fan
Diode in parallel:

       +---------|<------+         
+ -----+--xxxxx--\/\/\---+---------

Transister
============

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

Diodes
============

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
