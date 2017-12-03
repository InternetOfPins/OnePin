/* -*- C++ -*- */
#ifndef ONEBUTTON_DEF
  #define ONEBUTTON_DEF

  template<typename In,typename Out>
  struct Wire {
    static inline void link() {Out::set(In::in());}
    inline void operator()() const {return link();}
  };

  //TODO: check logic preservation on all cases

  /* use case
  typedef Wire<Btn,Led> BtnLed;
  void loop() {BtnLed::link();}//transfer the state

  or

  typedef Wire<Btn,Led> BtnLed;
  const BtnLed btnLed;
  void loop() {btnLed();}//transfer the stare

  */

#endif
