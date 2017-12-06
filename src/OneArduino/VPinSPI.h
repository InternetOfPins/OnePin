/* -*- C++ -*- */
#ifndef ARDUINO_VPINS_SPI_DEF_H
  #define ARDUINO_VPINS_SPI_DEF_H

  #include "OneLib.h"
  #include <Arduino.h>
  #include <SPI.h>

  // typedef uint8_t bit:1;

  namespace OneLib {
    namespace Arduino {

      template<SPIClass& spi,typename LatchPin,int sz=1>
      class SPIExt {
        public:
          static inline void begin() {
            LatchPin::begin();
            spi.begin();
          }
          template<uint8_t bit>
          static inline bool in() {return 1&(data[bit>>3]>>(0b111&bit));}
          template<uint8_t bit>
          static inline void on() {data[bit>>3]|=1<<(0b111&bit);}
          template<uint8_t bit>
          static inline void off() {data[bit>>3]&=~(1<<(0b111&bit));}
        protected:
          static uint8_t data[sz];
          static void io() {
            LatchPin::pulse();
            spi.transfer(data,sz);
          	LatchPin::pulse();
          }
      };

      template<SPIClass& spi,typename LatchPin,int sz>
      uint8_t SPIExt<spi,LatchPin,sz>::data[sz];

      template<class SPIPort,int pin>
      struct SPIPin:protected SPIPort {
        static inline void begin() {}
        static inline bool in() {SPIPort::io();return SPIPort::template in<pin>();}
        static inline void on() {SPIPort::template on<pin>();SPIPort::io();}
        static inline void off() {SPIPort::template off<pin>();SPIPort::io();}
      };

    };
  };


#endif
