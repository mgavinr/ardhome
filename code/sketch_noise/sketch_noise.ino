int outp_noisePin = 3;
int outp_switch = A3;

void setup() {
  // put your setup code here, to run once:
  pinMode(outp_noisePin, OUTPUT);
  pinMode(outp_switch, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
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
  delay(5000);
}
