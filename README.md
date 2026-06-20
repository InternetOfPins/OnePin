# OnePin

HAPI pin/port API terminals. Hardware implementations are in **OneChip**. Bit selection is in **OneBit**.

## Terminals

These are the outermost (user-facing) layer in the HAPI chain. All methods are silent stubs; hardware components below provide the real implementations.

### `InPin<U = unsigned char>` — read-only

```cpp
static U    get()   // read port value (port-width U)
static void act()   // ISR propagation (silent)
```

### `OutPin<U = unsigned char>` — write-only

```cpp
static void on()      // set pin high   (maskless; Mask<> below provides the mask)
static void off()     // set pin low
static void put(U v)  // write full port value
static void act()
```

### `IOPin<U = unsigned char>` — bidirectional

```cpp
static U    get()
static void on()
static void off()
static void put(U v)
static void act()
```

## Convenience aliases

```cpp
AvrInPin   = InPin<unsigned char>     // AVR 8-bit ports
AvrOutPin  = OutPin<unsigned char>
AvrIOPin   = IOPin<unsigned char>

Stm32InPin  = InPin<unsigned int>    // STM32 32-bit ports
Stm32OutPin = OutPin<unsigned int>
Stm32IOPin  = IOPin<unsigned int>
```

## Composition model

```
Terminal (InPin/OutPin/IOPin)   ← outermost, user API
  oneBit::Mask<Pins<N>>         ← bit selection; provides maskBits()
  HwPort (from OneChip)         ← register access: get/set/rawSet/rawClear
```

`on()`/`off()` never take a mask argument — the mask is baked in by `oneBit::Mask<>` below.
Idiomatic use is a single pin (`Pins<5>`); multi-bit masks (`Pins<4,5,6,7>`) work too — `on()` sets all selected bits.
`put()`/`get()` operate at full port width (`U`).

## Example

```cpp
#include <onePin/onePin.h>
#include <chips/avr/avrPort.h>
#include <OneBit.h>
using namespace onePin;
using namespace hw::avr;
using namespace oneBit;

using Led = APIOf<AvrOutPin, Mask<Pins<5>>, chip::PortB>;
Led led;
led.on();   // PB5 high
led.off();  // PB5 low
led.put(0xFF);  // full port high
```
