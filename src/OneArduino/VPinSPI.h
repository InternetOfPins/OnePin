/* -*- C++ -*- */
/*
This is the Arduino version of virtual SPI pins
allowing onePin to target shift register pins over Arduino hardware SPI

Rui Azevedo, Dec 2017
ruihfazevedo@gmail.com
*/

#ifndef ARDUINO_VPINS_SPI_DEF_H
  #define ARDUINO_VPINS_SPI_DEF_H

  // #include "OneLib.h"
  #include <Arduino.h>
  #include <SPI.h>
  #include <OneBit.h>

  namespace OneLib {
    namespace Arduino {

      template<SPIClass& spi,typename LatchPin,int sz=1,typename Value=API::Value>
      class SPIExt {
        public:
          enum Field {size=sz};
          using ValueDef=Value;
          using API=Arduino::API;
          static inline void begin() {
            LatchPin::begin();
            spi.begin();
          }
          template<uint8_t bit,uint8_t nbits=1>
          static inline bool in() {return OneBit::Bits<uint8_t,uint8_t*,data,bit,nbits>::get();}
          template<uint8_t bit,uint8_t nbits=1>
          static inline uint8_t get() {return OneBit::Bits<uint8_t,uint8_t*,data,bit,nbits>::get();}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void on() {OneBit::Bits<uint8_t,uint8_t*,data,bit,nbits>::on();}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void off() {OneBit::Bits<uint8_t,uint8_t*,data,bit,nbits>::off();}
          template<uint8_t bit,uint8_t nbits=1>
          static inline void set(uint8_t value) {OneBit::Bits<uint8_t,uint8_t*,data,bit,nbits>::set(value);}
        protected:
          static uint8_t data[sz];
          static void io() {
            LatchPin::pulse();
            spi.transfer(data,sz);
          	LatchPin::pulse();
          }
      };

      template<SPIClass& spi,typename LatchPin,int sz,typename Value>
      uint8_t SPIExt<spi,LatchPin,sz,Value>::data[sz];

      template<class SPIExt,int bit,uint8_t nbits=1,typename Value=API::Value>
      struct SPIPin:protected SPIExt {
        using ValueDef=Value;
        using API=Arduino::API;
        static inline void begin() {}
        static inline bool in() {SPIExt::io();return SPIExt::template in<bit,nbits>();}
        static inline uint8_t get() {SPIExt::io();return SPIExt::template get<bit,nbits>();}
        static inline void on() {SPIExt::template on<bit,nbits>();SPIExt::io();}
        static inline void off() {SPIExt::template off<bit,nbits>();SPIExt::io();}
        static inline void set(uint8_t value) {
          SPIExt::template set<bit,nbits>(value);SPIExt::io();
        }
      };

    };
  };


#endif
