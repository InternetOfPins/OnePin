/* -*- C++ -*- */
#ifndef ONELIB_ARDUINO_DEF
  #define ONELIB_ARDUINO_DEF

// #include "OneLib.h"

#include "HAL/Pin.h"
#include "Soft/Debounce.h"
#include "Soft/Wire.h"

  namespace OneLib {
    namespace Arduino {

      struct API {
        using Value=uint8_t;
        static inline unsigned long getMillis() {return millis();}
        static inline void delay_ms(unsigned long ms) {delay(ms);}
        static inline void delay_us(unsigned int us) {delayMicroseconds(us);}
      };

      template<const int pin,typename Value=API::Value>
      struct PinBase {
        using ValueDef=Value;
        using API=Arduino::API;
        static inline void begin() {}
        static inline void mode(const Value m) {pinMode(pin,m);}
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
      template<int pin,typename Value>
      using Pin=
        LastState<
          LogicPinBase<
            PinBase<
              pin<0?-pin:pin,
              Value
            >,
            pin<0
          >
        >;

      template<int pin,typename Value=API::Value>
      struct InputPin:public Pin<pin,Value> {
        using ValueDef=Value;
        using API=Arduino::API;
        static inline void begin() {if (pin<0) Pin<pin,Value>::modeInUp(); else Pin<pin,Value>::modeIn();}
      };
      template<int pin,typename Value=API::Value>
      struct OutputPin:public Pin<pin,Value> {
        using ValueDef=Value;
        using API=Arduino::API;
        static inline void begin() {Pin<pin,Value>::modeOut();}
      };
    };

  };//namespace OneLib
#endif
