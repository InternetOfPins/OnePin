/* -*- C++ -*- */
#ifndef ONEPIN_DEF
  #define ONEPIN_DEF

  namespace OneLib {

    // class Framework {
    //   public:
    //     inline unsigned long getMillis() {return cnt++;}
    //   protected:
    //     unsigned long cnt=0;
    // };

    enum PinMode {ModeOpen,ModeOut,ModeIn,ModeInUp};

    // template<bool v> struct Bool{enum:bool {value=v};};
    // //a pin that can not be used
    // template<bool v=false>
    // struct ErrorPin {
    //   ErrorPin() {static_assert(Bool<v>::value,"Can not use uninitialized pin.");}
    // };

    //void pin, use this if no pin is to be used ------------------------------
    struct VoidPin {
      inline operator bool() {return in();}
      static inline void begin() {}
      static inline void modeOut() {}
      static inline void modeIn() {}
      static inline void modeInUp() {}
      static inline void on() {}
      static inline void off() {}
      static inline bool in() {return false;}
      static inline bool rawIn() {return in();}
      static inline bool logicIn() {return in();}
      // static inline void setLast(bool) {}
    } voidPin;//or its objective version

    //-----------------------------------------------------------------------
    //if needed invert pin logic or be absent : constexpr^0
    template<class O,bool isOn>
    class LogicPinBase:public O {
      public:
        static inline bool in() {return O::in()^isOn;}
        static inline bool logicIn() {return in();}
        static inline void on() {isOn?O::off():O::on();}
        static inline void off() {isOn?O::on():O::off();}
    };

    // template<bool isOn>
    // struct LogicPinBase<ErrorPin,isOn>:public ErrorPin {
    //   LogicPinBase() {static_assert(getvalue<isOn>::value,"Can not use uninitialized pin.");}
    // };

    //store last pin state
    template<class O>
    class LastState:public O {
      protected:
        static inline bool getLast() {return lastState;}
        static inline bool setLast(bool v) {return lastState=v;}
        static bool lastState;
    };

    template<class O>
    bool LastState<O>::lastState;

    //pin state record, update last pin state after reading input
    template<class O>
    class RecState:public O/*,protected virtual LastState<O>*/ {
      public:
        //TODO: also record output changes!
        static inline bool in() {return O::setLast(O::in());}
        static inline void on() {O::on();O::setLast(true);}
        static inline void off() {O::off();O::setLast(false);}
    };

    //avoid self stack
    //the type IS stacked but the functionality is NOT
    template<class O>
    class RecState<RecState<O>>:public RecState<O> {};

    //attach an action to pin change (input)
    //when pin changes
    template<class O,void(*f)()>
    class OnChangeAction:public O/*,protected virtual LastState<O>*/ {
      public:
        OnChangeAction() {}
        static inline bool in() {
          bool n=O::in();
          if (n!=O::getLast()) f();
          return n;
        }
        static inline void on() {O::on();if (!O::getLast()) f();}
        static inline void off() {O::Off();if (O::getLast()) f();}
    };

    template<class O,void(*f)()>
    class OnChangeAction<RecState<O>,f>
      :public OnChangeAction<O,f> {};

    template<class O,void(*f)()>
    using OnChange=RecState<OnChangeAction<O,f>>;

    //when pin rises
    template<class O,void(*f)()>
    class OnRiseAction:public O/*,protected virtual LastState<O>*/ {
      public:
        static inline bool in() {
          bool n=O::in();
          if (n&&n!=O::getLast()) f();
          return n;
        }
        static inline void on() {O::on();if (!O::getLast()) f();}
        static inline void off() {O::off();}
    };

    template<class O,void(*f)()>
    class OnRiseAction<RecState<O>,f>
      :public OnRiseAction<O,f> {};

    template<class O,void(*f)()>
    using OnRise=RecState<OnRiseAction<O,f>>;

    //when pin falls
    template<class O,void(*f)()>
    class OnFallAction:public O/*,protected virtual LastState<O>*/ {
      public:
        static inline bool in() {
          bool n=O::in();
          if (!(n||n==O::getLast())) f();
          return n;
        }
        static inline void on() {O::on();}
        static inline void off() {O::off();if (O::getLast()) f();}
    };

    template<class O,void(*f)()>
    class OnFallAction<RecState<O>,f>
      :public OnFallAction<O,f> {};

    template<class O,void(*f)()>
    using OnFall=RecState<OnFallAction<O,f>>;

    //this is the top type layer of pin types, avoid duplication
    //TODO wire this on top of all even when using just type level!
    template<class O>
    struct PinCap:public O {
      static inline void begin() {O::begin();}
      static inline void tog() {set(!O::in());}
      template<bool T> static inline void set() {T?O::on():O::off();}//compiletime
      inline operator bool() {return O::in();}
      static inline void set(bool v) {v?O::on():O::off();}//runtime
    };

    //remove LastState functionality (no-one else used it)
    template<class O>
    struct PinCap<LastState<O>>:public O {};
    //dont self overlap functionality (however type overlaps)
    template<class O>
    struct PinCap<PinCap<O>>:public PinCap<O> {};
  }
#endif
