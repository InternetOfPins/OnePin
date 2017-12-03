/* -*- C++ -*- */
// #ifndef ONEPIN_DEF
//   #define ONEPIN_DEF

  // namespace OneLib {

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
      template<bool T>
      static inline void set() {T?on():off();}//compiletime
      static inline void set(bool v) {v?on():off();}//runtime
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
        static inline void tog() {O::set(!O::in());}
        inline operator bool() {return in();}
    };

    // template<bool isOn>
    // struct LogicPinBase<ErrorPin,isOn>:public ErrorPin {
    //   LogicPinBase() {static_assert(getvalue<isOn>::value,"Can not use uninitialized pin.");}
    // };

    //store last pin state
    class LastState {
      protected:
        inline bool getLast() {return lastState;}
        inline bool setLast(bool v) {return lastState=v;}
        bool lastState;
    };

    //pin state record, update last pin state after reading input
    template<class O>
    class RecState:public O,protected virtual LastState {
      public:
        inline bool in() {return setLast(O::in());}
    };

    //attach an action to pin change (input)
    //when pin changes
    template<class O,void(*f)()>
    class OnChange:public O,protected virtual LastState {
      public:
        OnChange() {}
        inline operator bool() {return in();}
        inline bool in() {
          bool n=O::in();
          if (n!=getLast()) f();
          return n;
        }
    };
    //when pin rises
    template<class O,void(*f)()>
    class OnRise:public O,protected virtual LastState {
      public:
        inline operator bool() {return in();}
        inline bool in() {
          bool n=O::in();
          if (n&&n!=O::getLast()) f();
          return n;
        }
    };
    //when pin falls
    template<class O,void(*f)()>
    class OnFall:public O,protected virtual LastState {
      public:
        inline operator bool() {return in();}
        inline bool in() {
          bool n=O::in();
          if (!(n||n==O::getLast())) f();
          return n;
        }
    };

  // }
// #endif
