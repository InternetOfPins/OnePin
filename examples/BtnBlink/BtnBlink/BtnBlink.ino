#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneAvr.h>
#include <OneArduino.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328;

void ledOn() {Serial.println("led is on");}
void ledOff() {Serial.println("led is off");}

//static hardware description
typedef PinCap<OnRise<OnFall<OutputPin<portB,5>,ledOff>,ledOn>> Led;//pin 13 on arduino
// typedef PinCap<OneLib::Arduino::Debouncer<InputPin<portD,-4>,10>> EncBtn;//with reverse logic included
typedef InputPin<portD,-4> EncBtn;//with reverse logic included

void setup() {
  // Serial.begin(115200);
  // while(!Serial);
  Led::begin();
  EncBtn::begin();
}

//toggles on/off as specified in ms, here using arduino function millis()
inline bool togFn(unsigned int on,unsigned int off) {return (millis()%(on+off))<on;}

//blink with no delay
void loop() {
  if (EncBtn::in()) Led::set(togFn(10,90));
  else Led::set(togFn(500,500));
}
