/* -*- C++ -*- */
/*
some aux functions to be included in a namespace where
pinToPort_table and pinToBit_table are defined
*/
constexpr static inline uint8_t pinToPort(int pin) {
  return pinToPort_table[pin<0?-pin:pin];
}
constexpr static inline uint8_t pinToBit(int pin) {
  return pin<0?-pinToBit_table[-pin]:pinToBit_table[pin];
}
template<int8_t pin,uint8_t sz=1>
using Pin =Avr::Pin<pinToPort(pin),pinToBit(pin),sz>;
template<int8_t pin,uint8_t sz=1>
using InputPin = Avr::InputPin<pinToPort(pin),pinToBit(pin),sz>;
template<int8_t pin,uint8_t sz=1>
using OutputPin =Avr::OutputPin<pinToPort(pin),pinToBit(pin),sz>;
