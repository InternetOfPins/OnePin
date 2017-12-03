/* -*- C++ -*- */
class OnePin {
  public:
    inline operator bool() {return in();}
    template<unsigned char M> void mode();
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
    virtual bool in()=0;
    virtual bool rawIn()=0;
    virtual bool logicIn()=0;
    template<bool T>
    inline void set() {T?on():off();}//compiletime
    inline void set(bool v) {v?on():off();}//runtime
};
template<>inline void OnePin::mode<ModeOut>() {modeOut();}
template<>inline void OnePin::mode<ModeIn>() {modeIn();}
template<>inline void OnePin::mode<ModeInUp>() {modeInUp();}

template<class O>
class OnePinHook:public OnePin  {
  public:
    // VPin():pin(O()) {}
    OnePinHook(O& o):pin(o) {}
    virtual void begin() {pin.begin();}
    virtual void modeOut() {pin.modeOut();}
    virtual void modeIn() {pin.modeIn();}
    virtual void modeInUp() {pin.modeInUp();}
    virtual void on() {pin.on();}
    virtual void off() {pin.off();}
    virtual bool in() {return pin.in();}
    virtual bool rawIn() {return pin.rawIn();}
    virtual bool logicIn() {return pin.logicIn();}
  protected:
    O& pin;
};
