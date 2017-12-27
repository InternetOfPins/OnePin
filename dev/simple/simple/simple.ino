#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneBit.h>
// #include <OneAvr.h>
//
// using namespace OneLib;
// using namespace OneLib::Avr::AtMega328;
// typedef Avr::OutputPin<portB,5,1> Led;

uint8_t tmp=0;
typedef OneBit::BitField<uint8_t,(uint8_t*)0x23,0,4> Led;

int main() {
  Led::on();
  return 0;
}
