/*
Using SPI port output
we can assign to multiple pins at the same time
bringing the power of direct port output to OnePin lib

on this example we blink the full shift-register at once
*/
#include <OneAvr.h>
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Arduino;

typedef PinCap<OutputPin<9>> LatchPin;
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins on shift register
typedef PinCap<SPIPin<SPI_8,0,8>> SPIChip0;//using full 8 bits of shift register

template<unsigned int on,unsigned int off>
inline bool tog() {return (millis()%(on+off))<on;}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  SPI_8::begin();
  PinCap<OutputPin<13>>::begin();
}

void loop() {
  SPIChip0::set(tog<500,500>()?0xFF:0x00);
}
