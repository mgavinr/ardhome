/*---------------------------------------
* Includes
*---------------------------------------*/
#include <IRSendRev.h>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

/*---------------------------------------
* My objects
*---------------------------------------*/
class myinoutIR {
    const byte _pin;

  public:
    myinoutIR(byte attachTo) :
      _pin(attachTo)
    {
    }

    ~myinoutIR()
    {
    }

    void setup() {
      Serial.print("IR read ");Serial.println(_pin);
      IR.Init(_pin);
    } 
    void loop(unsigned long milliseconds) {
      unsigned long beg_milliseconds = millis();
      unsigned long cur_milliseconds = millis();
      unsigned char dta[20];

      Serial.println("Checking for IR messages:");
      while((cur_milliseconds - beg_milliseconds) < milliseconds) {
        cur_milliseconds = millis();
        unsigned long ir_device = 0;
        unsigned long ir_button = 0;
        unsigned long ir_group = 0;
        
        if (IR.IsDta()) {               // get IR data
            // TODO some devices send the same code twice
            IR.Recv(dta);               // receive data to dta
            if (dta[BIT_DATA_LEN] == 4) {
              ir_device = (dta[BIT_DATA+1] + (dta[BIT_DATA+0] << 8)) & (0x0000FFFF);
              ir_button = (dta[BIT_DATA+3] + (dta[BIT_DATA+2] << 8)) & (0x0000FFFF);
            } else if (dta[BIT_DATA_LEN] == 6) {
              ir_device = (dta[BIT_DATA+1] + (dta[BIT_DATA+0] << 8)) & (0x0000FFFF);
              ir_group  = (dta[BIT_DATA+3] + (dta[BIT_DATA+2] << 8)) & (0x0000FFFF);
              ir_button = (dta[BIT_DATA+5] + (dta[BIT_DATA+4] << 8)) & (0x0000FFFF);
            }
            Serial.print("IR device=0x"); Serial.print(ir_device, HEX);
            Serial.print(" IR group="); Serial.print(ir_group, HEX);
            Serial.print(" IR button=0x"); Serial.println((unsigned long)ir_button, HEX);
        } 
      }
      Serial.println("Finished Checking for IR messages");
    }

    void send() {
      unsigned char dtaSend[20];
      dtaSend[BIT_LEN]        = 11;     // all data that needs to be sent
      dtaSend[BIT_START_H]    = 70;    // the logic high duration of "Start"
      dtaSend[BIT_START_L]    = 34;     // the logic low duration of "Start"
      dtaSend[BIT_DATA_H]     = 9;     // the logic "long" duration in the communication
      dtaSend[BIT_DATA_L]     = 25;     // the logic "short" duration in the communication

      dtaSend[BIT_DATA_LEN]   = 6;      // Number of data which will sent. 
      //If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

      //0x40 0x04 0x05 0x38 0x48 0x75
      dtaSend[BIT_DATA + 0]   = 0x40;      // data that will sent
      dtaSend[BIT_DATA + 1]   = 0x04;
      dtaSend[BIT_DATA + 2]   = 0x05;
      dtaSend[BIT_DATA + 3]   = 0x38;
      dtaSend[BIT_DATA + 4]   = 0x48;
      dtaSend[BIT_DATA + 5]   = 0x75;
      IR.Send(dtaSend, 38);
      Serial.println("IR Sent the data");
    }
};

/*---------------------------------------
* My vars
*---------------------------------------*/
myinoutIR irout(3);

/*---------------------------------------
* Code
*---------------------------------------*/
void setup() {
  Serial.begin(9600);
  irout.setup();
}

void loop() {
  irout.send();
  delay(5000); //Delay 2 sec. 
}
