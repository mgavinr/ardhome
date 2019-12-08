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
int out_mistEnablePin2 = 2;
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

void wait_active() 
{
  int mo = LOW;
  int lastmo = LOW;
  delay(oneminute);
  digitalWrite(outp_fanPin, HIGH);
  for(int l = 0; l <= 2; l++) {
    Serial.println("Wait active");
    lastmo = LOW;
    for(int i = 0; i < 30; i++) {
      delay(onesecond);  // 1 second
      mo = digitalRead(in_motionPin);
      if (mo == HIGH) { 
        lastmo = HIGH; 
      }
    }
    if (lastmo == HIGH) { 
      Serial.println("Wait active noise");
      make_noise();
    }
  }
  Serial.println("End Wait active");
}

void turn_all(int status)
{
  digitalWrite(out_ledPin, status);
  
  if (status == HIGH) {
    
    digitalWrite(out_light5EnablePin, LOW);
    digitalWrite(out_light0EnablePin, LOW);
    digitalWrite(out_mistEnablePin, LOW);
    digitalWrite(out_mistEnablePin2, LOW);
  } else {
        digitalWrite(out_light5EnablePin, HIGH);
    digitalWrite(out_light0EnablePin, HIGH);
    digitalWrite(out_mistEnablePin, HIGH);
    digitalWrite(out_mistEnablePin2, HIGH);

  }
  if (status == LOW) {
    tone(outp_noisePin, 35, 500);
    digitalWrite(outp_fanPin, LOW);
  } else {
    tone(outp_noisePin, 35, 50);
  }
}

void make_noise()
{
  int i = 0;
  for (i=0; i<20; i++) {
    tone(outp_noisePin, 35+i, 50);
    delay(70);
    digitalWrite(outp_switch, HIGH);
    delay(10);
    digitalWrite(outp_switch, LOW);
    delay(10);
    digitalWrite(outp_switch, HIGH);
    delay(10);
    digitalWrite(outp_switch, LOW);
  }
  tone(outp_noisePin, 35+i, 500);
  delay(500);
  for (i=20; i>0; i--) {
    tone(outp_noisePin, 35+i, 50);
    delay(50);
    digitalWrite(outp_switch, LOW);
  }
  tone(outp_noisePin, 35+i, 500);
  delay(onesecond);
}

/** 
 * SETUP
 */
void setup() {
  pinMode(in_motionPin, INPUT);
  // defaults are low
  pinMode(out_ledPin, OUTPUT);
  pinMode(outp_fanPin, OUTPUT);
  pinMode(outp_switch, OUTPUT);
  pinMode(out_mistEnablePin, OUTPUT);
  pinMode(out_mistEnablePin2, OUTPUT);
  pinMode(out_light5EnablePin, OUTPUT);
  pinMode(out_light0EnablePin, OUTPUT);
  Serial.begin(9600);
}

/** 
 *  MAIN
 */
void loop(){
        digitalWrite(out_light5EnablePin, HIGH);
    digitalWrite(out_light0EnablePin, HIGH);
    digitalWrite(out_mistEnablePin, HIGH);
    digitalWrite(out_mistEnablePin2, HIGH);

  current_motion = digitalRead(in_motionPin);
  if (current_motion == HIGH)  // check if the input is HIGH
  {            
    if (last_motion == LOW) 
    {
      Serial.println("Motion detected!");
      last_motion = HIGH;
      turn_all(HIGH);
      make_noise();
    }
    wait_active();
  } 
  else // LOW
  {
    if (last_motion == HIGH) 
    {
      last_motion = LOW;
      Serial.println("Motion ended!");
      turn_all(LOW);
    }
    delay(onesecond);
  }
}
