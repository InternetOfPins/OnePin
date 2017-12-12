/* -*- C++ -*- */
/*
This is the Arduino version of virtual SPI pins
allowing onePin to target shift register pins over Arduino hardware SPI

Rui Azevedo, Dec 2017
ruihfazevedo@gmail.com
*/

#ifndef ARDUINO_VPINS_SPI_DEF_H
  #define ARDUINO_VPINS_SPI_DEF_H

  #include "OneLib.h"
  #include <Arduino.h>
  #include <SPI.h>

  namespace OneLib {
    namespace Arduino {

      template<SPIClass& spi,typename LatchPin,int sz=1>
      class SPIExt {
        public:
          static inline void begin() {
            LatchPin::begin();
            spi.begin();
          }
          template<uint8_t bit,uint8_t nbits=1>
          static inline bool in() {return BitPart<bit,nbits>::get(data);}
          template<uint8_t bit,uint8_t nbits=1>
          static inline uint8_t get() {return BitPart<bit,nbits>::get(data);}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void on() {BitPart<bit,nbits>::on(data);}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void off() {BitPart<bit,nbits>::off(data);}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void set(uint8_t value) {BitPart<bit,nbits>::set(data,value);}
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

      template<class SPIExt,int pin,uint8_t nbits=1>
      struct SPIPin:protected SPIExt {
        static inline void begin() {}
        static inline bool in() {SPIExt::io();return SPIExt::template in<pin,nbits>();}
        static inline uint8_t get() {SPIExt::io();SPIExt::template get<pin,nbits>();}
        static inline void on() {SPIExt::template on<pin,nbits>();SPIExt::io();}
        static inline void off() {SPIExt::template off<pin,nbits>();SPIExt::io();}
        static inline void set(uint8_t value) {
          SPIExt::template set<pin,nbits>(value);SPIExt::io();
        }
      };

    };
  };


#endif
