#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;

template<unsigned int on,unsigned int off>
inline bool tog() {return (millis()%(on+off))<on;}

//static hardware description
typedef OutputPin<PortB,5> Led1;//pin 13 on arduino
typedef OutputPin<PortC,3> Led2;//pin A3 on arduino

void setup() {
  Led1::begin();
  Led2::begin();
}

void loop() {
  Led1::set(tog<10,90>());
  Led2::set(tog<500,500>());
}
