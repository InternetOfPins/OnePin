#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneBit.h>
using namespace OneBit;
#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr::AtMega328;
typedef Avr::OutputPin<portB,5,1> Led;

void setup() {
  Led::begin();
}
void loop() {
  Led::on();
  delay(10);
  Led::off();
  delay(90);
}
