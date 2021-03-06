/* -*- C++ -*- */
#ifndef ONELIB_AVR_DEF
  #define ONELIB_AVR_DEF

// #include "OneLib.h"
  #include <OneBit.h>
  #include "HAL/Pin.h"
  // #include "OneLib/Soft/Debounce.h"//avr has no millis functions, so cant have this
  #include "Soft/Wire.h"

  namespace OneLib {
    namespace Avr {

      struct API {
        using Value=uint8_t;
        // inline unsigned long getMillis() {static_assert(false,"AVR has no time elapse source by default, use a specific framework.");}//AVR has not this one!
        static inline void delay_ms(double ms) {_delay_ms(ms);}
        static inline void delay_us(double us) {_delay_ms(us);}
      };

      //access avr port registers from a base address
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using In = OneBit::Bits<uint8_t,size_t,base,at,sz>;
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using Mode = OneBit::Bits<uint8_t,size_t,base+1,at,sz>;
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using Out = OneBit::Bits<uint8_t,size_t,base+2,at,sz>;

      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      struct Port:
        protected In<base,at,sz>,
        protected Mode<base,at,sz>,
        protected Out<base,at,sz>
      {
        using API=Avr::API;
        using ValueDef=API::Value;
        static inline uint8_t mode() {return Mode<base,at,sz>::get();}
        static inline void mode(uint8_t m) {Mode<base,at,sz>::set(m);}
        static inline uint8_t in() {return In<base,at,sz>::get();}
        static inline void out(uint8_t data) {Out<base,at,sz>::set(data);}

        static inline void modeOut() {Mode<base,at,sz>::on();}
        static inline void modeIn() {
          Mode<base,at,sz>::off();
          Out<base,at,sz>::off();
        }
        static inline void modeInUp() {
          Mode<base,at,sz>::off();
          Out<base,at,sz>::on();
        }
        static inline void modeOff() {
          Mode<base,at,sz>::off();
          Out<base,at,sz>::off();
        }
        static inline uint8_t get() {return In<base,at,sz>::get();}
        static inline void set(uint8_t value) {Out<base,at,sz>::set(value);}
        static inline void on() {Out<base,at,sz>::on();}
        static inline void off() {Out<base,at,sz>::off();}
        static inline void tog() {Out<base,at,sz>::set(~In<base,at,sz>::get());}

        static inline void begin() {}
        static inline uint8_t rawIn() {return in();}
      };

      template<size_t addr,int pin,int8_t sz=1>
      struct PinBase:public Port<addr,pin,sz> {
        static inline void begin() {}
        static inline uint8_t rawIn() {return Port<addr,pin,sz>::in();}
      };

      template<size_t addr,int pin,uint8_t sz=1>
      using Pin=
        LastState<
          LogicPinBase<
            PinBase<
              addr,
              pin<0?-pin:pin,
              sz
            >,
            (pin<0)
          >
        >;

      //TODO: move begin to HAL/Pin.h avoiding the if!
      template<size_t addr,int pin,uint8_t sz=1>
      struct InputPin:public Pin<addr,pin,sz> {
        static inline void begin() {
          if (pin<0) Pin<addr,pin,sz>::modeInUp();
          else Pin<addr,pin,sz>::modeIn();
        }
      };

      template<size_t addr,int pin,uint8_t sz=1>
      struct OutputPin:public Pin<addr,pin,sz> {
        static inline void begin() {Pin<addr,pin,sz>::modeOut();}
      };

      //Example of MCU ports/pins zero-cost abstraction
      //all this defs go only on compile time
      namespace AtMega328 {
        enum Ports:size_t {
          portB=0x23,
          portC=0x26,
          portD=0x29
        };
        template<uint8_t at,int8_t sz=1>
        using PortB=Port<portB,at,sz>;
        namespace ArduinoPins {
          constexpr static uint8_t pinToPort_table[]={
            portD,portD,portD,portD,portD,portD,portD,portD,
            portB,portB,portB,portB,portB,
            portC,portC,portC,portC,portC
          };
          constexpr static uint8_t pinToBit_table[]={
            0,1,2,3,4,5,6,7,
            0,1,2,3,4,5,
            0,1,2,3,4,5
          };
          #include "ArduinoPins.h"
        };//namespace ArduinoPins
      };//namespace AtMega328p

      namespace AtTinyX5 {
        //TODO: check this info!
        enum Ports:uint8_t {portB=0x36};
        namespace ArduinoPins {
          constexpr static uint8_t pinToPort_table[]={portB,portB,portB,portB,portB};
          constexpr static uint8_t pinToBit_table[]={0,1,2,3,4};
          #include "ArduinoPins.h"
        };//namespace ArduinoPins
      };//namespace AtTiny

      namespace AtTinyX4 {
        //TODO: check this info!
        enum Ports:uint8_t {portB=0x36,portA=0x39};
        namespace ArduinoPins {
          constexpr static uint8_t pinToPort_table[]={
            portB,portB,portB,portB,portB,portB,
            portA,portA,portA,portA,portA
          };
          constexpr static uint8_t pinToBit_table[]={
            0,1,2,3,4,5,
            6,7,2,1,0

          };
          #include "ArduinoPins.h"
        };//namespace ArduinoPins
      };//namespace AtTiny

      namespace AtTinyX8 {
        //TODO: check this info!
        enum Ports:uint8_t {portB=0x23,portC=0x26,portD=0x29,portA=0x2C};
        namespace ArduinoPins {
          constexpr static uint8_t pinToPort_table[]={
            portD,portD,portD,portD,portD,portD,portD,portD,
            portB,portB,portB,portB,portB,portB,portB,portB,
            portC,portC,portC,portC,portC,portC,portC,
          };
          constexpr static uint8_t pinToBit_table[]={
            0,1,2,3,4,5,6,7,
            0,1,2,3,4,5,6,7,
            7,0,1,2,3,4,5
          };
          #include "ArduinoPins.h"
        };//namespace ArduinoPins
      };//namespace AtTiny

      namespace AtTiny13=AtTinyX5;
      namespace AtTiny25=AtTinyX5;
      namespace AtTiny45=AtTinyX5;
      namespace AtTiny85=AtTinyX5;
      namespace AtTiny44=AtTinyX4;
      namespace AtTiny84=AtTinyX4;
      namespace AtTiny48=AtTinyX8;
      namespace AtTiny88=AtTinyX8;

  }//avr namespace

  }//namespace OneLib
#endif
