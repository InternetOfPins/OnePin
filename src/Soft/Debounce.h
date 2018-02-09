/* -*- C++ -*- */

//debounce pin state change
template<class O,unsigned long delta>
class DebounceOnOff:public O/*,protected virtual LastState*/ {
  public:
    static inline typename O::Value in() {
      if (O::getMillis()-lastSet<delta) return O::getLast();
      lastSet=O::getMillis();
      return O::in();
    }
    // inline operator Value() {return in();}
  protected:
    static unsigned long lastSet;
};

template<class O,unsigned long delta>
unsigned long DebounceOnOff<O,delta>::lastSet=-delta;

template<class O,unsigned long delta>
class DebounceOnOff<RecState<O>,delta>
  :public DebounceOnOff<O,delta> {};

template<class O,unsigned long delta>
using Debouncer=RecState<DebounceOnOff<O,delta>>;
