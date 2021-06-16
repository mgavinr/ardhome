#define MYMAIN 0
#define MYSERIAL 0
#include "/tmp/a/MYRTC.h"
#include "/tmp/a/MYSEVEN.h"

MYRTC myrtc;
MYSEVEN myseven;
MYSEVEN myseven_info(1);
bool draw_dots = false;
char* seven_info = (char*)"time";

void setup() {
  Serial.begin(9600);
  myrtc.setup();
  myseven.setup();
  myseven_info.setup();
}

DateTime display_time() {
  int display_time = 0;
  DateTime now = myrtc._rtc.now();
  display_time = now.hour() * 100;
  display_time += now.minute();
  myseven._matrix.clear();
  myseven._matrix.print(display_time);
  myseven._matrix.drawColon(draw_dots);
  myseven._matrix.writeDisplay();  

  // Title
  //                     FSTOP MID TLEFT BLEFT BOT BRIGHT TRIGHT TOP
  //seven_info._matrix.writeDigitRaw(0, (uint8_t) 0b000000000XXXXXXX); 
  myseven_info._matrix.writeDigitRaw(0, (uint8_t) 0b0000000001111000); // t
  myseven_info._matrix.writeDigitRaw(1, (uint8_t) 0b0000000000010000); // i
  // COLON = 0x10  
  myseven_info._matrix.writeDigitRaw(2, (uint8_t) 0b0000000000000000); //   
  myseven_info._matrix.writeDigitRaw(3, (uint8_t) 0b0000000001010100); // m
  myseven_info._matrix.writeDigitRaw(4, (uint8_t) 0b0000000001111001); // E

  myseven_info._matrix.writeDisplay();  
  Serial.print("The time to display is="); Serial.println(display_time);
  return now;
}

void loop() {
  myrtc.loop();
  myseven.loop();
  myseven_info.loop();

  DateTime now = display_time(); 
  Serial.print("The time is currently=");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.println("");

  delay(2000);
  draw_dots = !draw_dots;
}
