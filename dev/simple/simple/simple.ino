#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneBit.h>
// #include <OneAvr.h>
//
// using namespace OneLib;
// using namespace OneLib::Avr::AtMega328;
// typedef Avr::OutputPin<portB,5,1> Led;

uint8_t tmp[4]={0,0,0,0};
typedef OneBit::Bits<uint8_t,tmp,7,2,uint8_t> Led;

int main() {
  Led::set(1);
  return Led::get();
}
