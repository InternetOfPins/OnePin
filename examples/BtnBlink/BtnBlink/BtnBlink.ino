#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;

void ledOn() {Serial.println("led is on");}
void ledOff() {Serial.println("led is off");}

//static hardware description
typedef PinCap<OnRise<OnFall<OutputPin<PortB,5>,ledOff>,ledOn>> Led;//pin 13 on arduino
typedef PinCap<InputPin<PortD,-4>> EncBtn;//with reverse logic included

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Led::begin();
  EncBtn::begin();
}

//toggles on/off as specified in ms
inline bool togFn(unsigned int on,unsigned int off) {return (millis()%(on+off))<on;}

//blink with no delay
void loop() {
  if (EncBtn()) Led::set(togFn(100,900));
  else Led::set(togFn(500,500));
  delay(100);
}
