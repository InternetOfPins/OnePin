#include <OneBit.h>

namespace OnePin {
  using namespace OneBit;
  struct API {
    static inline void delay_us(int us) {}
    static inline void delay_ms(int ms) {}
    static inline unsigned long ms() {}
  };

  template<typename Unit,typename Type,Type data,uint8_t at, uint8_t sz=1,typename Value=Unit>
  struct PinBase:public Bits<Unit,Type,data,at,sz,Value> {};

  struct VoidPin:public PinBase<void,NULL,NULL,0,0> {};
};

using namespace OnePin;
void setup() {}
void loop() {}
