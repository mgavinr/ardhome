#define MYMAIN 0
#define MYSERIAL 0
#include "/tmp/a/MYRTC.h"
#include "/tmp/a/MYSEVEN.h"
#include "/tmp/a/MYDEBUG.h"

/*  orig
Sketch uses 8288 bytes (25%) of program storage space. Maximum is 32256 bytes.
Global variables use 788 bytes (38%) of dynamic memory, leaving 1260 bytes for local variables. Maximum is 2048 bytes.
*/
/* not sure what i did, cleanup code
Sketch uses 9476 bytes (29%) of program storage space. Maximum is 32256 bytes.
Global variables use 794 bytes (38%) of dynamic memory, leaving 1254 bytes for local variables. Maximum is 2048 bytes.
*/
/* adding events
Sketch uses 9624 bytes (29%) of program storage space. Maximum is 32256 bytes.
Global variables use 828 bytes (40%) of dynamic memory, leaving 1220 bytes for local variables. Maximum is 2048 bytes.
*/
/* adding debug library
Sketch uses 10840 bytes (33%) of program storage space. Maximum is 32256 bytes.
Global variables use 1060 bytes (51%) of dynamic memory, leaving 988 bytes for local variables. Maximum is 2048 bytes.
*/
/* adding led per mode, and pot reader double
Sketch uses 10968 bytes (34%) of program storage space. Maximum is 32256 bytes.
Global variables use 1062 bytes (51%) of dynamic memory, leaving 988 bytes for local variables. Maximum is 2048 bytes.
 fixing the event loop 5000 if off, 1000 if on
Sketch uses 11256 bytes (34%) of program storage space. Maximum is 32256 bytes.
Global variables use 1132 bytes (55%) of dynamic memory, leaving 916 bytes for local variables. Maximum is 2048 bytes.
*/
// TYPES
enum emode {
  e_off = 0,
  e_time,
  e_start_time,
  e_end_time,
  e_on_day,
  e_end
};
// PINS
const short dpin_mode_button = 2;
const short dpin_intled = 13;
const short dpin_sdl = 19;
const short dpin_sda = 18;
const short dpin_time_off = 10;
const short dpin_time_on = 9;
const short dpin_time_day = 8;
const short apin_high = 3;
const short apin_pot = 2;
const short e_loop_delay_off_interval = 5000;
const short e_loop_delay_on_interval = 1000;
const short e_loop_delay_poll_interval = 250;
// VARS
MYRTC myrtc;
MYSEVEN myseven;
bool      g_draw_dots = false;
int       g_mode = e_off;
int       g_loop_delay_ms = e_loop_delay_on_interval;
int       g_potval = 0;

// WAIT: 0 
// DOES: calls event functions if pin changes
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
// TODO change this to an array of events like https://github.com/janelia-arduino/Vector/blob/master/src/Vector.h
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
// DOES: waits, unless the pin changes
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
void ldelay(int delay_ms, short dpin_watch) {
  if (delay_ms > 0) {
    DEBUG("Wait of:%d mili exactly",delay_ms);
    delay(delay_ms);  // short delay
    g_loop_delay_ms-=delay_ms;  
  } else {
    if (dpin_watch > 0) {
      // check for events and delay
      DEBUG("Wait of: %d mili seconds this loop", g_loop_delay_ms);
      int start_value = digitalRead(dpin_watch);
      int curr_value = start_value;
      while(curr_value == start_value) {
        curr_value = digitalRead(dpin_watch);
        //DEBUG("Pin value: %d", curr_value);
        delay(e_loop_delay_poll_interval);  // short delay
        g_loop_delay_ms-=e_loop_delay_poll_interval;  
        if(g_loop_delay_ms <= 0) break;
      }
      //DEBUG("Pin value: %d", curr_value);
      if (curr_value != start_value) {        
        levent(dpin_watch);        
      }
    } else {
      DEBUG("Wait of: %d mili seconds this loop no interrupt",g_loop_delay_ms);
      delay(g_loop_delay_ms);  // full delay
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
  DEBUG("Computed time=%d",display_time);
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
  digitalWrite(dpin_time_on, LOW);
  digitalWrite(dpin_time_off, LOW);
  digitalWrite(dpin_time_day, LOW);
      
  switch ((emode) g_mode)
  {
    case e_time: 
      DEBUG("The display mode is now: CURRENT TIME");
      break;
    case e_start_time: 
      DEBUG("The display mode is now: START TIME");
      digitalWrite(dpin_time_on, HIGH);
      break;
    case e_end_time: 
      DEBUG("The display mode is now: END TIME");
      digitalWrite(dpin_time_off, HIGH);
      break;
    case e_on_day: 
      DEBUG("The display mode is now: ON DAY");
      digitalWrite(dpin_time_day, HIGH);
      break;
    case e_off: 
      DEBUG("The display mode is now: OFF");
      break;
    default: 
      DEBUG("The display mode is now: unknown:%d", g_mode);
      break;
  }
}

void check_mode_value() {
  int last_potval = g_potval;
  g_potval = analogRead(apin_pot);
  DEBUG("************ The potval is now %d", g_potval);    
}

void setup() {
  Serial.begin(9600);
  DEBUG("--- SETUP ---");
  pinMode(dpin_mode_button, INPUT);
  pinMode(dpin_intled, OUTPUT);
  pinMode(dpin_time_on, OUTPUT);
  pinMode(dpin_time_off, OUTPUT);
  pinMode(dpin_time_day, OUTPUT);
  pinMode(apin_pot, INPUT);
  pinMode(apin_high, OUTPUT);
  digitalWrite(apin_high, HIGH);
  digitalWrite(dpin_intled, LOW);
  digitalWrite(dpin_time_on, LOW);
  digitalWrite(dpin_time_off, LOW);
  digitalWrite(dpin_time_day, LOW);
  g_potval = analogRead(apin_pot);
  myrtc.setup();
  myseven.setup();
  myseven.off();
  DEBUG("--- END SETUP ---");
  MYDEBUG::enabled = true;
}

void loop() {
  if(g_mode == e_off) {
    g_loop_delay_ms = e_loop_delay_off_interval;
    ldelay(0,dpin_mode_button);
  } else {
    // Display is on
    check_mode_value();
    g_loop_delay_ms = e_loop_delay_on_interval;
    myrtc.loop();  //empty
    myseven.loop();  //empty
    g_draw_dots = !g_draw_dots;
    DateTime now = display_time(); 
    DEBUG("The time is currently=%d:%d", now.hour(), now.minute());
    ldelay(0,dpin_mode_button);
  }
}
