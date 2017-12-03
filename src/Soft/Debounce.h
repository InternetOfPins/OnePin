/* -*- C++ -*- */

//debounce the `on` state of a pin -- do we need this?
template<class O,unsigned long delta>
class DebounceOn:public O,protected virtual LastState {
  public:
    inline bool in() {
      if (O::Framework::getMillis()-lastOn<delta) return true;
      else if (this->O::in()) {
        lastOn=O::Framework::getMillis();
        return true;
      }
      return false;
    }
    inline operator bool() {return in();}
  protected:
    unsigned long lastOn=-delta;
};

//-------------------------------------------------------------
//debounce pin state change
template<class O,unsigned long delta>
class Debounce:public O,protected virtual LastState {
  public:
    inline bool in() {
      if (getMillis()-lastSet<delta) return getLast();
      lastSet=getMillis();
      return O::in();
    }
    inline operator bool() {return in();}
  protected:
    unsigned long lastSet=-delta;
};

template<class O,unsigned long delta>
using Debouncer=Debounce<O,delta>;
