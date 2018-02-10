/* -*- C++ -*- */
/*
Pin manipulation meta-framework
OnePin meta-framework can overlay existing frameworks with zero cost

Along with the pin manipulation functions there are also a couple of composable
utilities like debouncing or action coupling

A top layer with virtual functions is provided for library abstracion over Pins
OnePin is this break layer, breaking appart the type level abstracion from  a generic high level

This is the base for IOP - The Internet Of Pins
on the sequence of VirtualPins proposals done to Arduino framework
*/

#ifndef ONE_PIN_DEF_H
  #define ONE_PIN_DEF_H
  #include <OneBit.h>
  #include "HAL/Pin.h"
  using Byte=OneBit::Byte;
  namespace OneLib {
    #include "HAL/Pin.h"
    template<typename Value,Byte sz=1>
    class OnePin {
      public:
        inline operator Value() {return in();}
        // template<unsigned char M> void mode();
        inline void mode(const PinMode m) {
          switch(m) {
            case ModeOut: modeOut();break;
            case ModeIn: modeIn();break;
            case ModeInUp: modeInUp();break;
            case ModeOpen: modeIn();off();break;
          }
        }
        virtual void begin()=0;
        virtual void modeOut()=0;
        virtual void modeIn()=0;
        virtual void modeInUp()=0;
        virtual void on()=0;
        virtual void off()=0;
        virtual void set(Value v)=0;
        virtual Value in()=0;
        virtual Value rawIn()=0;
        virtual Value logicIn()=0;
        // template<Value T>
        // inline void set() {T?on():off();}//compiletime
        // inline void set(Value v) {v?on():off();}//runtime
    };
    // template<>inline void OnePin::mode<ModeOut>() {modeOut();}
    // template<>inline void OnePin::mode<ModeIn>() {modeIn();}
    // template<>inline void OnePin::mode<ModeInUp>() {modeInUp();}

    template<class O,typename Value,Byte sz=1>
    class OnePinHook:public OnePin<Value,sz>  {
      public:
        OnePinHook(O& o):pin(o) {}
        inline void begin() override {pin.begin();}
        inline void modeOut() override {pin.modeOut();}
        inline void modeIn() override {pin.modeIn();}
        inline void modeInUp() override {pin.modeInUp();}
        inline void on() override {pin.on();}
        inline void off() override {pin.off();}
        inline Value in() override {return pin.in();}
        inline void set(Value v) override {pin.set(v);}
        inline Value rawIn() override {return pin.rawIn();}
        inline Value logicIn() override {return pin.logicIn();}
      protected:
        O& pin;
    };

    template<typename P,typename Value,Byte sz=1>
    class Hook {
    public:
      static P hwPin;
      static OnePinHook<P,Value,sz> hook;
      // inline operator OnePin&() {return hook;}
      static inline OnePin<Value,sz>& pin() {return hook;}
    };
    template<typename P,typename Value,Byte sz>
    P Hook<P,Value,sz>::hwPin;
    template<typename P,typename Value,Byte sz>
    OnePinHook<P,Value,sz> Hook<P,Value,sz>::hook(Hook<P,Value,sz>::hwPin);

  };//namespace OneLib

#endif
