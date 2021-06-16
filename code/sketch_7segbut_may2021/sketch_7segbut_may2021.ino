#define MYMAIN 0
#define MYSERIAL 0
#include "/tmp/a/MYRTC.h"
#include "/tmp/a/MYSEVEN.h"

/*
Sketch uses 8288 bytes (25%) of program storage space. Maximum is 32256 bytes.
Global variables use 788 bytes (38%) of dynamic memory, leaving 1260 bytes for local variables. Maximum is 2048 bytes.
*/

MYRTC myrtc;
MYSEVEN myseven;
MYSEVEN myseven_info(1);
bool draw_dots = false;
char* seven_info = (char*)"time";
const int button_pin = 2;
const int iled_pin = 13;
int button_state = 0;
enum emode {
  m_time = 0,
  m_on_time,
  m_off_time,
  m_on_day,
  m_end
};
int mode = m_time;


void setup() {
  Serial.begin(9600);
  myrtc.setup();
  myseven.setup();
  myseven_info.setup();
  pinMode(button_pin, INPUT);
  pinMode(iled_pin, OUTPUT);
  digitalWrite(iled_pin, LOW);
  mode = 0;
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

void display_mode() {
  switch ((emode) mode)
  {
    case m_time: 
      Serial.println("The display mode is: CURRENT TIME");
      break;
    case m_on_time: 
      Serial.println("The display mode is: ON TIME");
      break;
    case m_off_time: 
      Serial.println("The display mode is: OFF TIME");
      break;
    case m_on_day: 
      Serial.println("The display mode is: ON DAY");
      break;
    default: 
      Serial.println("The display mode is: unknown:");
      Serial.println(mode);
      break;
  }
}

void loop() {
  myrtc.loop();
  myseven.loop();
  myseven_info.loop();
  int loopcount = 10;
  digitalWrite(iled_pin, LOW);
  while(loopcount > 0) {
    loopcount--;
    button_state = digitalRead(button_pin);
    if (button_state == HIGH) {
      Serial.println("The button was pressed");
      digitalWrite(iled_pin, HIGH);
      mode++;
      if(mode == m_end) mode = 0;
      display_mode();
    } else {
      digitalWrite(iled_pin, LOW);
    }
    delay(200);
  }

  DateTime now = display_time(); 
  Serial.print("The time is currently=");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.println("");

  draw_dots = !draw_dots;
}
