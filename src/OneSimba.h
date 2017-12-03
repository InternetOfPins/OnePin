/* -*- C++ -*- */
#ifndef ONELIB_SIMBA_DEF
  #define ONELIB_SIMBA_DEF

  #include <simba.h>
  #include <drivers/pin.h>
  #include <time.h>
  #include "OneLib.h"

  namespace OneLib {
    namespace Simba {
      struct Framework {
        static unsigned long getMillis() {
          time_t now;
          time_get(&now);
          return now.nanoseconds/1000000+now.seconds*1000;
        }
      };
      inline unsigned long getMillis() {return Framework::getMillis();}
      template<int pin>
      struct PinBase:public Framework,public pin_driver_t {
        // using pin_driver_t::pin_driver_t;
        inline PinBase():pin_driver_t(pin) {}
        inline void begin() {}
        inline void mode(const int m) {pin_set_mode(this,m);}
        inline void modeOut() {mode(PIN_OUTPUT);}
        inline void modeIn() {mode(PIN_INPUT);}
        // inline void modeInUp() {mode(PIN_INPUT_PULLUP);}
        inline bool in() {return pin_read(this);}
        inline bool rawIn() {return in();}
        inline bool logicIn() {return in();}
        inline void on() {pin_write(this  ,1);}
        inline void off() {pin_write(this,0);}
        template<bool T>
        inline void set() {T?on():off();}
        inline void set(bool v) {v?on():off();}
      };
      template<int pin>
      using Pin=LogicPinBase<PinBase<pin<0?-pin:pin>,pin<0>;

      template<int pin>
      struct InputPin:public Pin<pin> {
        inline void begin() {
          if (pin<0) pin_init(this,pin,input_pull_up);//TODO: not been able to do this with simba framework
          else pin_init(this,pin,PIN_INPUT);
        }
      };
      // template<pin_device pin>
      struct OutputPin:public Pin<pin> {
        static inline void begin() {pin_init(this,pin,output);}
      };
    };
  }//namespace OneLib
#endif
