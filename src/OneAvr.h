/* -*- C++ -*- */
#ifndef ONELIB_AVR_DEF
  #define ONELIB_AVR_DEF

// #include "OneLib.h"

  namespace OneLib {
    namespace Avr {

      #include "HAL/Mem.h"
      #include "HAL/Func.h"
      #include "HAL/Pin.h"
      #include "OneBit.h"

      // #include "OneLib/Soft/Debounce.h"//avr has no millis functions, so cant have this
      #include "Soft/Wire.h"

      // #include "OnePin.h"

      //bit-fields base is one byte on avrs (8 bit mcus)
      template<size_t data,OneBit::Byte at=0, OneBit::Byte sz=1>
      using BitField=OneBit::BitField<uint8_t,data,at,sz>;

      //access avr port registers from a base address
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using In = BitField<base,at,sz>;
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using Mode = BitField<base+1,at,sz>;
      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      using Out = BitField<base+2,at,sz>;

      template<uint8_t base,uint8_t at=0,uint8_t sz=8>
      struct Port:
        protected In<base,at,sz>,
        protected Mode<base,at,sz>,
        protected Out<base,at,sz>
      {
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
        static inline void on() {Out<base,at,sz>::on();}
        static inline void off() {Out<base,at,sz>::off();}

        static inline void begin() {}
        static inline uint8_t rawIn() {return in();}
      };

      template<size_t addr,int pin,int8_t sz=1>
      struct PinBase:public Port<addr,pin,sz> {
        static inline void begin() {}
        static inline uint8_t rawIn() {return Port<addr,pin,sz>::in();}
      };

      template<size_t addr,int pin,uint8_t sz=1>
      using Pin=LastState<LogicPinBase<PinBase<addr,pin<0?-pin:pin,sz>,(pin<0)>>;

      //TODO: move begin to HAL/Pin.h avoiding the if!
      template<size_t addr,int pin,uint8_t sz>
      struct InputPin:public Pin<addr,pin,sz> {
        static inline void begin() {
          if (pin<0) Pin<addr,pin,sz>::modeInUp();
          else Pin<addr,pin,sz>::modeIn();
        }
      };

      template<size_t addr,int pin,uint8_t sz>
      struct OutputPin:public Pin<addr,pin,sz> {
        static inline void begin() {Pin<addr,pin,sz>::modeOut();}
      };

      // template<uint8_t* pinToPort_table,uint8_t* pinToBit_table>
      // struct ArduinoPins {
      //   constexpr static inline uint8_t pinToPort(int pin) {
      //     return pinToPort_table[pin<0?-pin:pin];
      //   }
      //   constexpr static inline uint8_t pinToBit(int pin) {
      //     return pin<0?-pinToBit_table[-pin]:pinToBit_table[pin];
      //   }
      //   template<int8_t pin,uint8_t sz=1>
      //   using Pin =Avr::Pin<pinToPort(pin),pinToBit(pin),sz>;
      //   template<int8_t pin,uint8_t sz=1>
      //   using InputPin = Avr::InputPin<pinToPort(pin),pinToBit(pin),sz>;
      //   template<int8_t pin,uint8_t sz=1>
      //   using OutputPin =Avr::OutputPin<pinToPort(pin),pinToBit(pin),sz>;
      // };

      //Example of MCU ports/pins zero-cost abstraction
      //all this defs go only on compile time
      namespace AtMega328 {
        enum Ports:uint8_t {portB=0x23,portC=0x26,portD=0x29};
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
