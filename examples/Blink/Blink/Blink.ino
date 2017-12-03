#include <OneArduino.h>

using namespace OneLib;
using namespace OneLib::Arduino;

typedef OutputPin<LED_BUILTIN> Led;

void setup() {
  Led::begin();
}

void loop() {
  Led::tog();
  delay(500);
}
