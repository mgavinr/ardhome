/*---------------------------------------*/
#include <stdarg.h>

/*---------------------------------------
* classes 
*---------------------------------------*/
class MYDEBUG {
  public:
    static bool enabled;
    void setup() {
    }
    void loop() {
    }
};

bool MYDEBUG::enabled = true;

void DEBUG(const char *fmt,  ...) {
  int dval;
  double fval;
  char *sval;
  if(MYDEBUG::enabled == false) return;
  //
  va_list stdargs;  /* points to each unnamed arg in turn */
  va_start(stdargs, fmt); /* make ap point to 1st unnamed arg */
  const char *c;
  for (c = fmt; *c; ++c) {
    if(*c == '%') {
      switch (*(++c) ) {
        case 'd':
          dval = va_arg(stdargs, int);
          Serial.print(dval);
          break;
        case 'f':
          fval = va_arg(stdargs, double);
          Serial.print(fval);
          break;
        case 's':
          sval = va_arg(stdargs, char*);
          Serial.print(sval);
          break;
        default:
          Serial.print(*c);
          break;
      };
    } else {
      Serial.print(*c);
    }
  }
  Serial.println();
  va_end(stdargs);
}


/*---------------------------------------
* MYMAIN
*---------------------------------------*/
#if MYMAIN == 1
MYDEBUG mydebug;

void setup() {
  Serial.begin(9600);
  mydebug.setup();
}

void loop() {
  mydebug.loop();
  DEBUG("Hello world");
  DEBUG("Hello %d world", 1);
  DEBUG("Hello %f world", 1.1);
  DEBUG("Hello %s world", "brave new");
  delay(5000); 
}
#endif
