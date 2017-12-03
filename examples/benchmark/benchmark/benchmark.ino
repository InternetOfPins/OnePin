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

unsigned long test(const char* title,auto test0,auto test1,auto test2) {
  Serial.print("|");
  Serial.print(title);
  Serial.print(":\t\t|");
  Serial.print(test(test0));
  Serial.print("\t|");
  Serial.print(test(test1));
  Serial.print("\t|");
  Serial.print(test(test0));
  Serial.println("\t|");
}

void test() {
  Serial.println("**Benchmark**");
  Serial.println("### Control");//------------------------------------
  Serial.println("| Test                          |\#Ops  |Debounced|OnePin   |");
  Serial.println("|------------------------------:|------:|------:|------:|");
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  Serial.println("### pinMode");//------------------------------------
  Serial.println("| Test                          |\#Ops  |Debounced|OnePin   |");
  Serial.println("|------------------------------:|------:|------:|------:|");
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin mode",
    [](){pinMode(13,INPUT);},
    [](){},
    [](){}
  );
  test("OneLib Arduino mode",
    [](){Pin13::modeIn();},
    [](){RecState<Debouncer<Pin13,30>>::modeIn();},
    [](){pin13.modeIn();}
  );
  test("OneLib AVR mode",
    [](){Led::modeIn();},
    [](){RecState<Debouncer<Led,30>>::modeIn();},
    [](){led.modeIn();}
  );
  test("OneLib VoidPin mode",
    [](){VoidPin::modeIn();},
    [](){RecState<Debouncer<VoidPin,30>>::modeIn();},
    [](){vvoidPin.modeIn();}
  );

  Serial.println("### digitalRead");//------------------------------------
  Serial.println("| Test                          |\#Ops  |Debounced|OnePin   |");
  Serial.println("|------------------------------:|------:|------:|------:|");
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin input",
    [](){digitalRead(13);},
    [](){},
    [](){}
  );
  test("OneLib Arduino input",
    [](){Pin13().in();},
    [](){RecState<Debouncer<Pin13,30>>().in();},
    [](){pin13.in();}
  );
  test("OnePin AVR input",
    [](){Led().in();},
    [](){RecState<Debouncer<Led,30>>().in();},
    [](){led.in();}
  );
  test("OneLib VoidPin input",
    [](){VoidPin().in();},
    [](){RecState<Debouncer<VoidPin,30>>().in();},
    [](){vvoidPin.in();}
  );

  Serial.println("### digitalWrite");//------------------------------------
  Serial.println("| Test                          |\\#Ops  |Debounced|OnePin   |");
  Serial.println("|------------------------------:|------:|------:|------:|");
  test("Empty function",
    [](){},
    [](){},
    [](){}
  );
  test("Arduino pin output",
    [](){digitalWrite(13,0);},
    [](){},
    [](){}
  );
  test("OneLib Arduino output",
    [](){Pin13::off();},
    [](){RecState<Debouncer<Pin13,30>>::off();},
    [](){pin13.off();}
  );
  test("OnePin AVR output",
    [](){Led::off();},
    [](){RecState<Debouncer<Led,30>>::off();},
    [](){led.off();}
  );
  test("OneLib VoidPin output",
    [](){VoidPin::off();},
    [](){RecState<Debouncer<VoidPin,30>>::off();},
    [](){vvoidPin.off();}
  );
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  test();
}

void loop() {}
