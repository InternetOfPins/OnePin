/* -*- C++ -*- */
#ifndef ONELIB_ARDUINO_DEF
  #define ONELIB_ARDUINO_DEF

// #include "OneLib.h"

  namespace OneLib {
    namespace Arduino {

      struct API {
        static inline unsigned long getMillis() {return millis();}
        static inline void delay_ms(unsigned long ms) {delay(ms);}
        static inline void delay_us(unsigned int us) {delayMicroseconds(us);}
      };

      using Value=uint8_t;

      // #include <OneBit.h>
      #include "HAL/Pin.h"
      #include "Soft/Debounce.h"
      #include "Soft/Wire.h"

      template<const int pin>
      struct PinBase {
        static inline void begin() {}
        static inline void mode(const uint8_t m) {pinMode(pin,m);}
        static inline void modeOut() {pinMode(pin,OUTPUT);}
        static inline void modeIn() {pinMode(pin,INPUT);}
        static inline void modeInUp() {pinMode(pin,INPUT_PULLUP);}
        static inline Value in() {return digitalRead(pin);}
        static inline Value rawIn() {return digitalRead(pin);}
        static inline Value logicIn() {return digitalRead(pin);}
        static inline void on() {digitalWrite(pin,HIGH);}
        static inline void off() {digitalWrite(pin,LOW);}
        template<Value T>
        static inline void set() {T?on():off();}
        static inline void set(Value v) {v?on():off();}
        static inline void tog() {in()?off():on();}
      };
      template<int pin>
      using Pin=LastState<LogicPinBase<PinBase<pin<0?-pin:pin>,pin<0>>;

      template<const int pin>
      struct InputPin:public Pin<pin> {
        static inline void begin() {if (pin<0) Pin<pin>::modeInUp(); else Pin<pin>::modeIn();}
      };
      template<const int pin>
      struct OutputPin:public Pin<pin> {
        static inline void begin() {Pin<pin>::modeOut();}
      };
    };

  };//namespace OneLib
#endif
