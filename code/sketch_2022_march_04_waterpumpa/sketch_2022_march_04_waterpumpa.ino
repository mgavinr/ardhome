#define MYMAIN 0
#define MYSERIAL 0
#define LOOP_SLEEP_OFF 10000    // just has to be bigger than:
#define LOOP_SLEEP 1500    // blink clock every second
#define LOOP_BUTTON_LONG 15
#define LOOP_BUTTON_MIN 5
#define LOOP_BUTTON_SHORT 5
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
 adding rtc time setting and sprintf time string ..
 Sketch uses 13146 bytes (40%) of program storage space. Maximum is 32256 bytes.
Global variables use 1260 bytes (61%) of dynamic memory, leaving 788 bytes for local variables. Maximum is 2048 bytes.

2023
- fix the button pressing, change the delay functions
Sketch uses 11534 bytes (35%) of program storage space. Maximum is 32256 bytes.
Global variables use 1180 bytes (57%) of dynamic memory, leaving 868 bytes for local variables. Maximum is 2048 bytes.
- add back the edit, bit of a jump now!! :(
Sketch uses 13742 bytes (42%) of program storage space. Maximum is 32256 bytes.
Global variables use 1348 bytes (65%) of dynamic memory, leaving 700 bytes for local variables. Maximum is 2048 bytes.
- fix the edit button for time, add some averaging pots
Sketch uses 14312 bytes (44%) of program storage space. Maximum is 32256 bytes.
Global variables use 1435 bytes (70%) of dynamic memory, leaving 613 bytes for local variables. Maximum is 2048 bytes.

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
// VARS
MYRTC myrtc;                        // internal objs
MYSEVEN myseven;                    // internal objs
//
DateTime  g_draw_time;              // display the time
bool      g_edit_mode = false;      // long press => stay in edit mode
bool      g_draw_dots = false;      // display the :
int       g_loop_delay_ms = LOOP_SLEEP;  // loop interval is 1 second
int       g_button_high = 0;        // interval button was pressed for
int       g_mode = e_off;           // startup is off
int       g_potval = 0;             // 0 to 1000



// WAIT: 0 
// DOES: get current time from rtc and display it
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
void display_time() {
  int display_time = 0;
  display_time = g_draw_time.hour() * 100;
  display_time += g_draw_time.minute();
  DEBUG("Computed time=%d",display_time);
  myseven._matrix.clear();
  myseven._matrix.print(display_time);
  myseven._matrix.drawColon(g_draw_dots);
  myseven._matrix.writeDisplay();
}

// WAIT: 500 
// DOES: sets the mode flashes led 
// USES: g_mode
// RETV:
void next_mode_code() {
  digitalWrite(dpin_intled, HIGH);
  ldelay(200);
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
      g_draw_time = myrtc._rtc.now();
      DEBUG("The time is currently=%d:%d", g_draw_time.hour(), g_draw_time.minute());
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

/*
Sketch uses 14200 bytes (44%) of program storage space. Maximum is 32256 bytes.
Global variables use 1435 bytes (70%) of dynamic memory, leaving 613 bytes for local variables. Maximum is 2048 bytes.

or with averages

Sketch uses 14312 bytes (44%) of program storage space. Maximum is 32256 bytes.
Global variables use 1435 bytes (70%) of dynamic memory, leaving 613 bytes for local variables. Maximum is 2048 bytes.

Do you like my rudementary maths?  I think there are no bugs here right, right you bet

*/
int analogMyRead(short port) {
  int potval1, potval2, potval3, potval4 = 0;
  int potvalt1, potvalt2, potvalt3, potvalt4 = 0;
  int potvalt = 0;
  int potvalr = 0;
  potval1 = analogRead(apin_pot);
  ldelay(10);
  potval2 += analogRead(apin_pot);
  ldelay(10);
  potval3 += analogRead(apin_pot);
  ldelay(10);
  potval4 += analogRead(apin_pot);
  ldelay(10);
  potvalt1 = (potval2 + potval3 + potval4)*2;
  potvalt2 = (potval1 + potval3 + potval4)*2;
  potvalt3 = (potval1 + potval2 + potval4)*2;
  potvalt4 = (potval1 + potval2 + potval3)*2;
  // Pick the highest one ..
  potvalt = potvalt1;
  if (potvalt1 < potvalt2) potvalt = potvalt2;
  if (potvalt1 < potvalt3) potvalt = potvalt3;
  if (potvalt1 < potvalt4) potvalt = potvalt4;
  //
  if (potvalt2 < potvalt3) potvalt = potvalt3;
  if (potvalt2 < potvalt4) potvalt = potvalt4;
  //
  if (potvalt3 < potvalt4) potvalt = potvalt4;
  return (potvalt/6);
}

void edit_mode_code() {
  int last_potval = g_potval;
  g_potval = analogMyRead(apin_pot);

  int time24 = g_potval * 2;
  int time24_hour = time24 / 100;
  int time24_minute = time24 - (time24_hour * 100);
  int time24_seconds = 0;
  if(time24_hour > 23) time24_hour = 23;
  if(time24_minute > 59) time24_minute = 59;
  char time24_string[] = "00:00:00";
  sprintf(time24_string, "%02d:%02d:00", time24_hour, time24_minute);
  DEBUG("[ADJUST] potval=%d", g_potval);
  DEBUG("[......] time24=%d", time24);
  DEBUG("[......] time24_hour=%d", time24_hour);
  DEBUG("[......] time24_minute=%d", time24_minute);
  DEBUG("[......] time24_string=%s", time24_string);
  DEBUG("[......] date=%s", __DATE__);

  switch ((emode) g_mode)
  {
    case e_time: 
      myrtc._rtc.adjust(DateTime(__DATE__, time24_string));
      DEBUG("[ADJUST] rtc=%s", time24_string);
      g_draw_time = myrtc._rtc.now();
      break;
    case e_start_time: 
    case e_end_time: 
    case e_on_day: 
      break;
    default: 
      break;
  }
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

// WAIT: 0 
// DOES: waits, unless the pin changes
// USES: g_draw_dots
// RETV: DateTime.hour .minute 
void ldelay(int delay_ms) {
  DEBUG("ldelay(%d ms)",delay_ms);
  delay(delay_ms);  // short delay
  g_loop_delay_ms-=delay_ms;  
  DEBUG("loop=%dms", g_loop_delay_ms);
}

/* 
** loop_sleep - polls the watch pin, it is a busy loop reading the pin every 100ms, it runs endlessly
** Waiting = forever and 100ms
** Globals = 
** Returns =
*/
void loop_sleep(int maxwait, short watch_pin) {
  int curr_value = HIGH;    // off
  DEBUG("loop_sleep polling for max %d ms", maxwait);
  while(maxwait > 0) {
    curr_value = digitalRead(watch_pin);
    // nothing, noone is pressing
    if (curr_value == HIGH) {
      if (g_button_high > 0) {        
        curr_value -= 1;
      }
      // noone is pressing, can we return something?
      // 10 is kinda like a debouncer right?
      if (g_button_high > LOOP_BUTTON_MIN) {
        DEBUG("Switch was on %d times, off now", g_button_high);
        loop_event(watch_pin);
        return;
      }
    }
    // someone is pressing, keep adding
    if (curr_value == LOW) {
      DEBUG("Switch still on %d times", g_button_high);
      g_button_high += 1;
    }
    delay(100);
    maxwait -= 100;
  }
  DEBUG("loop_sleep polling done");
}

/* 
** loop_event - a wakeup event has happened, at the moment this means the button was pressed, I may expand it to cover other events, it actually updates the display to correspond to the current mode.
** Waiting = none
** Globals = 
** Returns = 
*/
void loop_event(short dpin) {
  if(dpin == dpin_mode_button) {
    if ((g_button_high >= LOOP_BUTTON_SHORT) && (g_button_high < LOOP_BUTTON_LONG)) {
      // short press
      DEBUG("!! Mode button pressed");
      if (g_edit_mode) {
        DEBUG("Leave edit mode, next mode");
        edit_mode_code();
        g_edit_mode = false;
      } else {
        next_mode_code();
      }
    }
    if ((g_button_high >= LOOP_BUTTON_LONG) || (g_edit_mode == true)) {
      if (g_edit_mode == false) { DEBUG("!! Edit Mode button pressed"); }
      // long press
      g_edit_mode = true;
      edit_mode_code();
      ldelay(100);
      myseven._matrix.clear();
      myseven._matrix.writeDisplay();
      ldelay(100);
      display_time(); 
      edit_mode_code();
    } 
    if(g_mode == e_off) {
      myseven._matrix.clear();
      myseven._matrix.writeDisplay();
    }
  }
  g_button_high = 0;
}

void loop() {
  if(g_edit_mode) {
      // stay in this mode
      DEBUG("In edit mode");
      myrtc.loop();  //empty
      myseven.loop();  //empty
      g_draw_dots = !g_draw_dots;
      display_time(); 
      loop_event(dpin_mode_button); // <---- here
      loop_sleep(g_loop_delay_ms, dpin_mode_button);
      g_loop_delay_ms = LOOP_SLEEP;
  } else {
    if(g_mode == e_off) {
      // loop forever really
      loop_sleep(LOOP_SLEEP_OFF, dpin_mode_button);
    } else {
      // Display is on
      // check_mode_value();
      myrtc.loop();  //empty
      myseven.loop();  //empty
      g_draw_dots = !g_draw_dots;
      display_time(); 
      loop_sleep(g_loop_delay_ms, dpin_mode_button);
      g_loop_delay_ms = LOOP_SLEEP;
    }
  }
}
