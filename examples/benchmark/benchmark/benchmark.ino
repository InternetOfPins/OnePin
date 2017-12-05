#include <OneAvr.h>
#include <OneArduino.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;
// using namespace OneLib::Arduino;

#define ENCBTN_PIN -4

typedef Avr::Pin<PortB,5> Led;//pin 13 on arduino
Led _led;
OnePinHook<Led> led(_led);

typedef Arduino::Pin<13> Pin13;
Pin13 _pin13;
OnePinHook<Pin13> pin13(_pin13);

OnePinHook<VoidPin> vvoidPin(voidPin);

unsigned long cnt;
unsigned long start;
unsigned long duration=20000;
unsigned long test(auto f) {
  cnt=0;
  unsigned long lim=duration+micros();
  while(micros()<lim) {
    f();
    cnt++;
  }
  return cnt;
}

bool valid[4][5][4]={
  {
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1}
  },
  {
    {0,0,0,0},
    {0,1,0,0},
    {0,1,1,1},
    {0,1,1,1},
    {0,1,0,1}
  },
  {
    {0,0,0,0},
    {0,1,0,0},
    {0,1,1,1},
    {0,1,1,1},
    {0,1,0,1}
  },
  {
    {0,0,0,0},
    {0,1,0,0},
    {0,1,1,1},
    {0,1,1,1},
    {0,1,0,1}
  },
};

unsigned long test(int grp,int idx,const char* title,auto test0,auto test1,auto test2,auto test3) {
  Serial.print("|");
  Serial.print(title);
  Serial.print(":\t\t|");
  int r0=test(test0);
  if (valid[grp][idx][0]) Serial.print(r0); else Serial.print("-");
  Serial.print("\t|");
  int r1=test(test1);
  if (valid[grp][idx][1]) Serial.print(r1); else Serial.print("-");
  Serial.print("\t|");
  int r2=test(test2);
  if (valid[grp][idx][2]) Serial.print(r2); else Serial.print("-");
  Serial.print("\t|");
  int r3=test(test3);
  if (valid[grp][idx][3]) Serial.print(r3); else Serial.print("-");
  Serial.println("\t|");
}

void head() {
  Serial.println("| Test                          |Empty  |\#Ops   |Deb.   |OnePin |");
  Serial.println("|------------------------------:|------:|------:|------:|------:|");
}

void test() {
  Serial.println("**Benchmark**");
  Serial.println("### Control");//------------------------------------
  head();//--------------------------------------------------------------------
  test(0,0,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(0,1,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(0,2,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(0,3,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(0,4,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  Serial.println("### pinMode");//------------------------------------
  head();//--------------------------------------------------------------------
  test(1,0,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(1,1,"Arduino pin mode",
    [](){},
    [](){pinMode(13,INPUT);},
    [](){},
    [](){}
  );
  test(1,2,"OneLib Arduino mode",
    [](){},
    [](){Pin13::modeIn();},
    [](){PinCap<OneLib::Arduino::Debouncer<Pin13,10>>::modeIn();},
    [](){pin13.modeIn();}
  );
  test(1,3,"OneLib AVR mode",
    [](){},
    [](){Led::modeIn();},
    [](){PinCap<OneLib::Arduino::Debouncer<Led,10>>::modeIn();},//yup raw Avr does not have a soft debouncer... it requires timming, still we can use arduiino timming
    [](){led.modeIn();}
  );
  test(1,4,"OneLib VoidPin mode",
    [](){},
    [](){VoidPin::modeIn();},
    [](){},//PinCap<OneLib::Arduino::Debouncer<VoidPin,10>>::modeIn();},
    [](){vvoidPin.modeIn();}
  );

  Serial.println("### digitalRead");//------------------------------------
  head();//--------------------------------------------------------------------
  test(2,0,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(2,1,"Arduino pin input",
    [](){},
    [](){digitalRead(13);},
    [](){},
    [](){}
  );
  test(2,2,"OneLib Arduino input",
    [](){},
    [](){Pin13().in();},
    [](){PinCap<OneLib::Arduino::Debouncer<Pin13,10>>::in();},
    [](){pin13.in();}
  );
  test(2,3,"OnePin AVR input",
    [](){},
    [](){Led().in();},
    [](){PinCap<OneLib::Arduino::Debouncer<Led,10>>().in();},
    [](){led.in();}
  );
  test(2,4,"OneLib VoidPin input",
    [](){},
    [](){VoidPin().in();},
    [](){},//PinCap<OneLib::Arduino::Debouncer<VoidPin,10>>().in();},
    [](){vvoidPin.in();}
  );

  Serial.println("### digitalWrite");//------------------------------------
  head();//--------------------------------------------------------------------
  test(3,0,"Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test(3,1,"Arduino pin output",
    [](){},
    [](){digitalWrite(13,0);},
    [](){},
    [](){}
  );
  test(3,2,"OneLib Arduino output",
    [](){},
    [](){Pin13::off();},
    [](){PinCap<OneLib::Arduino::Debouncer<Pin13,10>>::off();},
    [](){pin13.off();}
  );
  test(3,3,"OnePin AVR output",
    [](){},
    [](){Led::off();},
    [](){PinCap<OneLib::Arduino::Debouncer<Led,10>>::off();},
    [](){led.off();}
  );
  test(3,4,"OneLib VoidPin output",
    [](){},
    [](){VoidPin::off();},
    [](){},//PinCap<OneLib::Arduino::Debouncer<VoidPin,10>>::off();},
    [](){vvoidPin.off();}
  );
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  test();
}

void loop() {}
