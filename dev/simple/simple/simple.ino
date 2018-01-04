#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr::AtMega328;

typedef PortB<5> Led;

void setup() {
  Led::begin();
}
void loop() {
  Led::on();
  delay(10);
  Led::off();
  delay(90);
}
