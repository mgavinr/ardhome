// PWM pins 3, 5, 6, 9, 10, and 11
int led = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(led, 10);
}
