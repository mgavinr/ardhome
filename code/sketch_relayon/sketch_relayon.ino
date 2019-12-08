/**
 * Creeper by Gavin Rogers 2019
 */

// Contents
// PIR sensor
// FAN
// 24V Enable
// Light
// Sound maker (todo)
// External Light source
// PWM pins 3, 5, 6, 9, 10, and 11

/**
 * Inputs
 */
int in_motionPin = 8;

/**
 * Outputs
 */
int out_ledPin = 11;
int out_mistEnablePin = 7;
int out_light5EnablePin = A4;
int out_light0EnablePin = A5;
int outp_fanPin = 10;
int outp_noisePin = 3;
int outp_switch = A3;

/**
 * Global Variables
 */
int current_motion = LOW;
int last_motion = LOW;
int pwm_value = 127; // 0=off0% 255=on100% .. 63=25% 127=50% 190=75%
unsigned long oneminute = 10000;
unsigned long onesecond = 1000;
unsigned long thirtyseconds = 30000;


/** 
 * SETUP
 */
void setup() {
  pinMode(out_light5EnablePin, OUTPUT);
  pinMode(out_light0EnablePin, OUTPUT);
  pinMode(out_mistEnablePin, OUTPUT);
  Serial.begin(9600);
}

/** 
 *  MAIN
 */
void loop(){
  digitalWrite(out_light5EnablePin, LOW);
  digitalWrite(out_light0EnablePin, LOW);
  digitalWrite(out_mistEnablePin, HIGH);
}
