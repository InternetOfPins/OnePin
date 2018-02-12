/* -*- C++ -*- */
/*
*/
#ifndef HAL_PIN_H
#define HAL_PIN_H
  enum PinMode {ModeOpen,ModeOut,ModeIn,ModeInUp};

  #ifdef DEBUG
    #define NAMED(x) constexpr static inline const char* name() {return x;}
  #else
    #define NAMED(x)
  #endif

  //void pin, use this if no pin is to be used ------------------------------
  template<typename Value>
  struct VoidPin {
    NAMED("VoidPin")
    enum FieldSize {value=0};
    inline operator Value() {return in();}
    static inline void begin() {}
    static inline void modeOut() {}
    static inline void modeIn() {}
    static inline void modeInUp() {}
    static inline void on() {}
    static inline void off() {}
    static inline Value in() {return false;}
    static inline Value rawIn() {return in();}
    static inline Value logicIn() {return in();}
    // static inline void setLast(typename O::ValueDef) {}
  };

  //-----------------------------------------------------------------------
  //if needed invert pin logic or be absent : constexpr^0
  template<class O,typename O::ValueDef isOn>
  class LogicPinBase:public O {
    public:
      NAMED("LogicPinBase")
      static inline typename O::ValueDef in() {return O::in()^isOn;}
      static inline typename O::ValueDef logicIn() {return in();}
      static inline void on() {isOn?O::off():O::on();}
      static inline void off() {isOn?O::on():O::off();}
  };

  //store last pin state
  template<class O>
  class LastState:public O {
    public: NAMED("LastState")
    protected:
      static inline typename O::ValueDef getLast() {return lastState;}
      static inline typename O::ValueDef setLast(typename O::ValueDef v) {return lastState=v;}
      static typename O::ValueDef lastState;
  };

  template<class O>
  typename O::ValueDef LastState<O>::lastState;

  //pin state record, update last pin state after reading input
  template<class O>
  class RecState:public O/*,protected virtual LastState<O>*/ {
    public:
      NAMED("RecState")
      //TODO: also record output changes!
      static inline typename O::ValueDef in() {return O::setLast(O::in());}
      static inline void on() {O::on();O::setLast(true);}
      static inline void off() {O::off();O::setLast(false);}
      static inline void set(typename O::ValueDef v) {O::set(v);O::setLast(v);}
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
      NAMED("OnChangeAction")
      OnChangeAction() {}
      static inline typename O::ValueDef in() {
        typename O::ValueDef n=O::in();
        if (n!=O::getLast()) f();
        return n;
      }
      static inline void set(typename O::ValueDef v) {
        O::set(v);
        if (v!=O::getLast()) f();
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
      NAMED("OnRiseAction")
      static inline typename O::ValueDef in() {
        typename O::ValueDef n=O::in();
        if (n&&n!=O::getLast()) f();
        return n;
      }
      // static inline void set(typename O::ValueDef v) {
      //   O::set(v);
      //   if (v&&v!=O::getLast()) f();
      // }
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
      NAMED("OnFallAction")
      static inline typename O::ValueDef in() {
        typename O::ValueDef n=O::in();
        if (!(n||n==O::getLast())) f();
        return n;
      }
      static inline void set(typename O::ValueDef v) {
        O::set(v);
        if (!(v||(v==O::getLast()))) f();
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
    NAMED("PinCap")
    static inline void begin() {O::begin();}
    static inline void tog() {set(!O::in());}
    static inline void pulse() {tog();tog();}
    static inline void set(typename O::ValueDef v) {O::set(v);}
  };

  //remove LastState functionality (no-one else used it)
  template<class O>
  struct PinCap<LastState<O>>:public O {};
  //dont self overlap functionality (however type overlaps)
  template<class O>
  struct PinCap<PinCap<O>>:public PinCap<O> {};

  template<typename O,typename ... OO>
  struct PinGroup {
    NAMED("PinGroup")
    using Value=typename O::ValueDef;
    enum Field {size=O::Field::size+PinGroup<OO...>::Field::size};
    inline operator Value() {return in();}
    static inline void begin() {O::begin();PinGroup<OO ...>::begin();}
    static inline void modeOut() {O::modeOut();PinGroup<OO ...>::modeOut();}
    static inline void modeIn() {O::modeIn();PinGroup<OO ...>::modeIn();}
    static inline void modeInUp() {O::modeInUp();PinGroup<OO ...>::modeInUp();}
    static inline void on() {O::on();PinGroup<OO ...>::on();}
    static inline void off() {O::off();PinGroup<OO ...>::off();}
    static inline Value in() {return (O::in()<<PinGroup<OO...>::Field::size)|(PinGroup<OO ...>::in());}
    static inline Value rawIn() {return in();}
    static inline Value logicIn() {return in();}
  };

  template<typename O>
  struct PinGroup<O> {
    NAMED("PinGroup(end)")
    using Value=typename O::ValueDef;
    enum Field {size=O::Field::size};
    static inline void begin() {O::begin();}
    static inline void modeOut() {O::modeOut();}
    static inline void modeIn() {O::modeIn();}
    static inline void modeInUp() {O::modeInUp();}
    static inline void on() {O::on();}
    static inline void off() {O::off();}
    static inline Value in() {return O::in();}
    static inline Value rawIn() {return in();}
    static inline Value logicIn() {return in();}
  };

#endif
