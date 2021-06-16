/*---------------------------------------*/
#include "RTClib.h"

/*---------------------------------------
* classes 
*---------------------------------------*/
class MYRTC {
  public:
    RTC_DS3231 _rtc;
    char _daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    MYRTC() { }
    ~MYRTC() { }

    void setup() {
#if MYSERIAL == 1
      while (!Serial);
#endif
      Serial.println("MYRTC.setup()");
      if (!_rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
      }
      if (_rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // _rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      }
    }

    void loop() {
    }
};

/*---------------------------------------
* MYMAIN
*---------------------------------------*/
#if MYMAIN == 1
MYRTC myrtc;

void setup() {
  Serial.begin(9600);
  myrtc.setup();
}

void loop() {
  myrtc.loop();
  DateTime now = myrtc._rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(myrtc._daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
  DateTime future (now + TimeSpan(7,12,30,6));

  Serial.print(" now + 7d + 12h + 30m + 6s: ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.print("Temperature: ");
  Serial.print(myrtc._rtc.getTemperature());
  Serial.println(" C");

  Serial.println();
  delay(3000);
}
#endif
