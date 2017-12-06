#include <SPI.h>
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Arduino;

//us pin 9 a latch (transfer to output)
typedef PinCap<OutputPin<9>> LatchPin;
//describe SPI shift registers geometry
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins

//allocate optput pins from SPI shift registers SPI_8
typedef PinCap<SPIPin<SPI_8,0>> SPILed;

void setup() {
  SPI_8::begin();
}

void loop() {
  SPILed::on();
  delay(100);
  SPILed::off();
  delay(900);
}
