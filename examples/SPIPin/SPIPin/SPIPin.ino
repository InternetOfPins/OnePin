/*
On this example we use arduino framework hardware SPI port wired to an SPI shift-register

this is the inspiration base for VirtualPins, we then can define a pin type that corresponds
to the shift register output pins and use that abstraction as a regular pin.

*/

#include <SPI.h>
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Arduino;

//use pin 9 as latch (transfer to output)
typedef PinCap<OutputPin<9>> LatchPin;
//describe SPI shift registers geometry
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins

//allocate output pins from SPI shift registers SPI_8, bit 0
//for the nature of the SPI register used this are output only
//however the SPI protocol allows IO
typedef PinCap<SPIPin<SPI_8,0>> SPILed;
typedef PinCap<SPIPin<SPI_8,1>> SPILed;
typedef PinCap<SPIPin<SPI_8,2>> SPILed;
typedef PinCap<SPIPin<SPI_8,3>> SPILed;
typedef PinCap<SPIPin<SPI_8,4>> SPILed;
typedef PinCap<SPIPin<SPI_8,5>> SPILed;
typedef PinCap<SPIPin<SPI_8,6>> SPILed;
typedef PinCap<SPIPin<SPI_8,7>> SPILed;

template<class
class Record<

void setup() {
  SPI_8::begin();
}

void loop() {
  SPILed::on();
  delay(100);
  SPILed::off();
  delay(900);
}
