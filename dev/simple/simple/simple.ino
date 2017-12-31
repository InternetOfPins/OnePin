#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneBit.h>
using namespace OneBit;

// #include <OneAvr.h>

// using namespace OneLib;
// using namespace OneLib::Avr::AtMega328;

// typedef PortB<5> Led;
uint8_t tmp[]={0,0,0};
typedef Bits<uint8_t,uint8_t*,tmp,0> Tmp0;

typedef Bits<uint8_t,size_t,0x0023,0,8> PortB_IN;

void setup() {
  // Led::begin();
}
void loop() {
  // Led::on();
  // delay(10);
  // Led::off();
  // delay(90);
}
