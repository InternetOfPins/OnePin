/*
On this example we use arduino framework hardware SPI port wired to an SPI shift-register

this is the inspiration base for VirtualPins, we then can define a pin type that corresponds
to the shift register output pins and use that abstraction as a regular pin.

*/

#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Arduino;

//use pin 9 as latch (transfer to output)
typedef PinCap<OutputPin<9>> LatchPin;
//describe SPI shift registers geometry
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins

//allocate output pins from SPI shift registers SPI_8,
// starting at bit 0 and with size of 8 bits
//for the nature of the SPI register used this are output only
//however the SPI protocol allows IO
typedef PinCap<SPIPin<SPI_8,0,8>> SPILed;

void setup() {
  //just make sure that SPI activity does not jam programming
  delay(500);
  //will init SPI and latch pin
  SPI_8::begin();
}

void loop() {
  SPILed::on();
  delay(100);
  SPILed::off();
  delay(900);
}
