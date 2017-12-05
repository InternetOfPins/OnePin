#include <OneAvr.h>
#include <OneArduino.h>

using namespace OneLib;
// using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;
using namespace OneLib::Arduino;

#define ENCBTN_PIN -4

typedef Avr::Pin<PortB,5> Led;//pin 13 on arduino
Led _led;
OnePinHook<Led> led(_led);

typedef Pin<13> Pin13;
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

unsigned long test(const char* title,auto test0,auto test1,auto test2,auto test3) {
  Serial.print("|");
  Serial.print(title);
  Serial.print(":\t\t|");
  Serial.print(test(test0));
  Serial.print("\t|");
  Serial.print(test(test1));
  Serial.print("\t|");
  Serial.print(test(test2));
  Serial.print("\t|");
  Serial.print(test(test3));
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
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  Serial.println("### pinMode");//------------------------------------
  head();//--------------------------------------------------------------------
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin mode",
    [](){},
    [](){pinMode(13,INPUT);},
    [](){},
    [](){}
  );
  test("OneLib Arduino mode",
    [](){},
    [](){Pin13::modeIn();},
    [](){Debouncer<Pin13,10>::modeIn();},
    [](){pin13.modeIn();}
  );
  test("OneLib AVR mode",
    [](){},
    [](){Led::modeIn();},
    [](){Debouncer<Led,10>::modeIn();},
    [](){led.modeIn();}
  );
  test("OneLib VoidPin mode",
    [](){},
    [](){VoidPin::modeIn();},
    [](){Debouncer<VoidPin,10>::modeIn();},
    [](){vvoidPin.modeIn();}
  );

  Serial.println("### digitalRead");//------------------------------------
  head();//--------------------------------------------------------------------
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin input",
    [](){},
    [](){digitalRead(13);},
    [](){},
    [](){}
  );
  test("OneLib Arduino input",
    [](){},
    [](){Pin13().in();},
    [](){Debouncer<Pin13,10>().in();},
    [](){pin13.in();}
  );
  test("OnePin AVR input",
    [](){},
    [](){Led().in();},
    [](){Debouncer<Led,10>().in();},
    [](){led.in();}
  );
  test("OneLib VoidPin input",
    [](){},
    [](){VoidPin().in();},
    [](){Debouncer<VoidPin,10>().in();},
    [](){vvoidPin.in();}
  );

  Serial.println("### digitalWrite");//------------------------------------
  head();//--------------------------------------------------------------------
  test("Empty function",
    [](){},
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin output",
    [](){},
    [](){digitalWrite(13,0);},
    [](){},
    [](){}
  );
  test("OneLib Arduino output",
    [](){},
    [](){Pin13::off();},
    [](){Debouncer<Pin13,10>::off();},
    [](){pin13.off();}
  );
  test("OnePin AVR output",
    [](){},
    [](){Led::off();},
    [](){Debouncer<Led,10>::off();},
    [](){led.off();}
  );
  test("OneLib VoidPin output",
    [](){},
    [](){VoidPin::off();},
    [](){Debouncer<VoidPin,10>::off();},
    [](){vvoidPin.off();}
  );
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  test();
}

void loop() {}
