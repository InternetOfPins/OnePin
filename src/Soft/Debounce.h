/* -*- C++ -*- */

// namespace OneLib {
  //debounce the `on` state of a pin -- do we need this?
  /*template<class O,unsigned long delta>
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
  };*/

  //-------------------------------------------------------------
  //debounce pin state change
  template<class O,unsigned long delta>
  class DebounceOnOff:public O/*,protected virtual LastState*/ {
    public:
      static inline bool in() {
        if (API::getMillis()-lastSet<delta) return O::getLast();
        lastSet=API::getMillis();
        return O::in();
      }
      // inline operator bool() {return in();}
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
// };
