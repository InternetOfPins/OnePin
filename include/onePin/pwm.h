#pragma once
#include <hapi/hapi.h>

namespace onePin {

  // Accel<Lo, Hi> — bounded counter with symmetric acceleration.
  // up()/down() accelerate in the respective direction; reversal halves accel.
  // relax() damps acceleration (call when no key is held).
  template<int Lo = 0, int Hi = 255>
  struct Accel {
    template<typename O>
    struct Part : O {
      int _value = Lo;
      int _accel = 0;

      int value() const { return _value; }

      int up() {
        (_accel >= 0) ? ++_accel : (_accel /= -2);
        _value += _accel;
        if (_value > Hi) _value = Hi;
        return _value;
      }

      int down() {
        (_accel <= 0) ? --_accel : (_accel /= -2);
        _value += _accel;
        if (_value < Lo) _value = Lo;
        return _value;
      }

      void relax() { _accel /= 2; }
      void reset(int v = Lo) { _value = v; _accel = 0; }
    };
  };

  // PWMChan<PwmPin> — PWM output channel with Accel-driven dimming.
  //
  // Boundary optimisation (preserves original attiny85rf behaviour):
  //   value == Lo  → disable PWM, GPIO off   (no timer overhead at zero)
  //   value == Hi  → disable PWM, GPIO on    (full brightness without PWM flicker)
  //   otherwise    → write OCR, enable PWM
  //
  // PwmPin must provide static:
  //   begin()          — configure GPIO + timer
  //   set(uint8_t n)   — write OCR register
  //   enable()         — set COM bits (connect timer to pin)
  //   disable()        — clear COM bits (release pin to GPIO)
  //   on()             — GPIO full-on  (active-low → LOW, active-high → HIGH)
  //   off()            — GPIO full-off (active-low → HIGH, active-high → LOW)
  template<typename PwmPin, int Lo = 0, int Hi = 255>
  struct PWMChan {
    template<typename O>
    struct Part : Accel<Lo, Hi>::template Part<O> {
      using Base = typename Accel<Lo, Hi>::template Part<O>;

      static void begin() { PwmPin::begin(); }

      void up()   { set(Base::up()); }
      void down() { set(Base::down()); }
      void relax(){ Base::relax(); }

      void set(int n) {
        if      (n <= Lo) { PwmPin::disable(); PwmPin::off(); }
        else if (n >= Hi) { PwmPin::disable(); PwmPin::on(); }
        else              { PwmPin::set(uint8_t(n)); PwmPin::enable(); }
      }
    };
  };

} // namespace onePin
