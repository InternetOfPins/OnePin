#ifdef DEBUG
  #include <streamFlow.h>
#endif

#include <OneAvr.h>
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Avr;
// using namespace OneLib::Avr::AtMega328p;
using namespace OneLib::Arduino;

// use arduino pins over spi
typedef Avr::PinCap<Avr::OutputPin<AtMega328p::PortB,1>> LatchPin;
typedef Arduino::SPIExt<SPI, LatchPin> SPI_8;//a set of 8 output pins on shift register
// typedef PinCap<Arduino::SPIPin<SPI_8,0,8>> SPIChip0;//using full 8 bits of shift register

template<unsigned int on,unsigned int off>
inline bool tog() {return (millis()%(on+off))<on;}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  SPI_8::begin();
  Serial<<"OnePin dev simple"<<endl;
}

void loop() {
  // SPIChip0::set(tog<1000,1000>()?0xFF:0x00);
  Arduino::SPIPin<SPI_8,0,8>::set(tog<1000,1000>()?0xFF:0x00);
  // delay(100);
}
