#define MYMAIN 0
#define MYSERIAL 0
#include "/tmp/a/MYRTC.h"
#include "/tmp/a/MYSEVEN.h"

/*
Sketch uses 8288 bytes (25%) of program storage space. Maximum is 32256 bytes.
Global variables use 788 bytes (38%) of dynamic memory, leaving 1260 bytes for local variables. Maximum is 2048 bytes.
*/
/*
Sketch uses 9476 bytes (29%) of program storage space. Maximum is 32256 bytes.
Global variables use 794 bytes (38%) of dynamic memory, leaving 1254 bytes for local variables. Maximum is 2048 bytes.
*/
/*
Sketch uses 9624 bytes (29%) of program storage space. Maximum is 32256 bytes.
Global variables use 828 bytes (40%) of dynamic memory, leaving 1220 bytes for local variables. Maximum is 2048 bytes.
*/
// TYPES
enum emode {
  e_off = 0,
  e_time,
  e_on_time,
  e_off_time,
  e_on_day,
  e_end
};
// PINS
const short dpin_mode_button = 2;
const short dpin_intled = 13;
const short dpin_sdl = 19;
const short dpin_sda = 18;
const short e_loop_delay_max_interval = 5000;
const short e_loop_delay_check_interval = 500;
// VARS
MYRTC myrtc;
MYSEVEN myseven;
bool      g_draw_dots = false;
int       g_mode = e_off;
int       g_interval = e_loop_delay_max_interval;

void DEBUG2(const char* debugs, int debugi) {
  Serial.print(debugs);Serial.println(debugi);
}
void DEBUG(const char* debug) {
  Serial.println(debug);
}
void DEBUG(int debug) {
  Serial.println(debug);
}

// WAIT: 0 
// DOES: calls event functions if pin changes
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
// TODO change this to an array of events
void levent(short dpin) {
  if(dpin == dpin_mode_button) {
    DEBUG("Mode button pressed");
    next_mode();
    if(g_mode == e_off) {
        myseven._matrix.clear();
        myseven._matrix.writeDisplay();
    }
  }
}

// WAIT: 0 
// DOES: get current time from rtc and display it
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
void ldelay(int dvalue, short dpin_watch) {
  if (dvalue > 0) {
    DEBUG2("Wait of:",dvalue);
    delay(dvalue);  // short delay
    g_interval-=dvalue;  
  } else {
    if (dpin_watch > 0) {
      // check for events and delay
      DEBUG2("Wait some of:",g_interval);
      int start_value = digitalRead(dpin_watch);
      int curr_value = start_value;
      while(curr_value == start_value) {
        curr_value = digitalRead(dpin_watch);
        //DEBUG2("Pin value:", curr_value);
        delay(e_loop_delay_check_interval);  // short delay
        g_interval-=e_loop_delay_check_interval;  
        if(g_interval <= 0) break;
      }
      //DEBUG2("Pin value:", curr_value);
      if (curr_value != start_value) {        
        levent(dpin_watch);        
      }
    } else {
      DEBUG2("Wait all of:",g_interval);
      delay(g_interval);  // full delay
    }
  }
}

// WAIT: 0 
// DOES: get current time from rtc and display it
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
DateTime display_time() {
  int display_time = 0;
  DateTime now = myrtc._rtc.now();
  display_time = now.hour() * 100;
  display_time += now.minute();
  DEBUG2("Computed time=",display_time);
  myseven._matrix.clear();
  myseven._matrix.print(display_time);
  myseven._matrix.drawColon(g_draw_dots);
  myseven._matrix.writeDisplay();
  return now;
}

// WAIT: 500 
// DOES: sets the mode flashes led 
// USES: g_mode
// RETV:
void next_mode() {
  digitalWrite(dpin_intled, HIGH);
  ldelay(500,0);
  digitalWrite(dpin_intled, LOW);
  g_mode++;  
  if(g_mode == e_end) g_mode = 0;
      
  switch ((emode) g_mode)
  {
    case e_time: 
      DEBUG("The display mode is now: CURRENT TIME");
      break;
    case e_on_time: 
      DEBUG("The display mode is now: ON TIME");
      break;
    case e_off_time: 
      DEBUG("The display mode is now: OFF TIME");
      break;
    case e_on_day: 
      DEBUG("The display mode is now: ON DAY");
      break;
    default: 
      DEBUG2("The display mode is now: unknown:", g_mode);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  DEBUG("--- SETUP ---");
  pinMode(dpin_mode_button, INPUT);
  pinMode(dpin_intled, OUTPUT);
  digitalWrite(dpin_intled, LOW);
  myrtc.setup();
  myseven.setup();
  myseven.off();
  DEBUG("--- END SETUP ---");
}

void loop() {
  g_interval = e_loop_delay_max_interval;
  if(g_mode != e_off) {
    myrtc.loop();  //empty
    myseven.loop();  //empty
    g_draw_dots = !g_draw_dots;
    DateTime now = display_time(); 
    Serial.print("The time is currently=");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.println("");
  } 
  ldelay(0,dpin_mode_button);
  delay(1000);
}