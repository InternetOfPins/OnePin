#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;

typedef OutputPin<PortB,5> Led;

void setup() {
  Led::begin();
}

void loop() {
  Led::tog();
  _delay_ms(200);
}
