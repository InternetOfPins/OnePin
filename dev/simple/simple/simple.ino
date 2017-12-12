#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneAvr.h>
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
// using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;
using namespace OneLib::Arduino;

//use arduino pins over spi
typedef PinCap<OutputPin<9>> LatchPin;
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins on shift register
typedef PinCap<SPIPin<SPI_8,0>> SPILed;//using shift register pin 0

template<unsigned int on,unsigned int off>
inline bool tog() {return (millis()%(on+off))<on;}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  SPI_8::begin();
  Serial<<"OnePin dev simple"<<endl;
  PinCap<OutputPin<13>>::begin();
}

void loop() {
  SPILed::set(tog<1000,1000>());
  delay(100);
}
