# OnePin API Reference

GPIO pin abstraction layer. Provides compile-time abstraction over platform-specific pin access.

## Quick Start

```cpp
#include <chips/avr/avrDevice.h>
using namespace onePin;

// Define a pin
using MyLed = AVR::OutPin<Pins<5>, chip::PortB>;
MyLed led;

// Use it
led.dir(1<<5);     // Set as output
led.on();          // High
led.off();         // Low
led.toggle();      // Flip state
```

---

## Core Types

### Pin Masks & Descriptors

| Type | Purpose |
|------|---------|
| `Pins<N>` | Single-pin mask descriptor |
| `Pins<N, M, ...>` | Multi-pin mask descriptor |
| `Mask<Pins<N>>` | Bit mask wrapper for HAPI composition |

### Pin Directions

| Return | Function | Params | Description |
|--------|----------|--------|-------------|
| `void` | `pin.dir(uint8_t m)` | mask | Set pin direction (1=out, 0=in) |
| `void` | `pin.dir_out(uint8_t m)` | mask | Configure pins as outputs |
| `void` | `pin.dir_in(uint8_t m)` | mask | Configure pins as inputs |
| `uint8_t` | `pin.ddr()` | – | Read direction register |

### Pin Output Operations

| Return | Function | Params | Description |
|--------|----------|--------|-------------|
| `void` | `pin.on()` | – | Set pin(s) high |
| `void` | `pin.off()` | – | Set pin(s) low |
| `void` | `pin.toggle()` | – | Flip pin state |
| `void` | `pin.set(uint8_t v)` | value | Set pin state (1 or 0) |
| `void` | `pin.put(uint8_t v)` | byte | Write full port value |

### Pin Input Operations

| Return | Function | Params | Description |
|--------|----------|--------|-------------|
| `uint8_t` | `pin.pin()` | – | Read PIN register (current GPIO level) |
| `uint8_t` | `pin.port()` | – | Read PORT register (output latch) |
| `uint8_t` | `pin.get()` | – | Read current pin state |

---

## Pin Abstractions by Platform

### AVR

**Header**: `<chips/avr/avrDevice.h>`

| Type | Purpose |
|------|---------|
| `AVR::OutPin<Mask, Port>` | Digital output |
| `AVR::InPin<Mask, Port>` | Digital input (pull-up capable) |
| `AVR::IOPin<Mask, Port>` | Bidirectional pin |
| `AVR::InvOutPin<Mask, Port>` | Inverted output (active-low) |

**Port Aliases**:
- `chip::PortB`, `chip::PortC`, `chip::PortD` (ATmega)
- `chip::PortA` (ATmega1284)
- `chip::PortE`..`chip::PortL` (ATmega2560)

### ESP32

**Header**: `<chips/esp32/esp32Device.h>`

| Type | Purpose |
|------|---------|
| `ESP32::OutPin<Mask, GPIO>` | GPIO digital output |
| `ESP32::InPin<Mask, GPIO>` | GPIO digital input |
| `ESP32::IOPin<Mask, GPIO>` | Bidirectional GPIO |
| `ESP32::AnalogIn<ADC, Channel>` | ADC analog input |

### STM32

**Header**: `<chips/stm32/stm32Device.h>`

| Type | Purpose |
|------|---------|
| `STM32::OutPin<Mask, Port>` | GPIO push-pull output |
| `STM32::InPin<Mask, Port>` | GPIO input with pull-up |
| `STM32::IOPin<Mask, Port>` | Bidirectional GPIO |
| `STM32::InvOutPin<Mask, Port>` | Inverted output (open-drain) |

---

## Pull-Up & Input Configuration

| Feature | AVR | ESP32 | STM32 |
|---------|-----|-------|-------|
| Pull-up resistor | `InPin` auto-enables via PORT bit | GPIO config flag | GPIO config |
| Pull-down resistor | N/A | GPIO config flag | GPIO config |
| High-impedance input | `InPin` with PORT=0 | GPIO config | GPIO config |

---

## Bit Masking with OneBit

Use with `oneBit::Mask<>` for compile-time mask generation:

```cpp
#include <oneBit/oneBit.h>
using namespace oneBit;

// Multi-pin mask
using Mask345 = Mask<Pins<3, 4, 5>>;
led.dir(Mask345::value);  // Set PB3, PB4, PB5 as outputs
```

---

## Compile-Time Constraints

**Port Allocation**: OnePin enforces compile-time pin conflict detection

```cpp
// ERROR: Pin 5 used twice
using Led1 = AVR::OutPin<Pins<5>, chip::PortB>;
using Led2 = AVR::OutPin<Pins<5>, chip::PortB>;  // Conflict!
```

---

## Integration with OneBit

OnePin composes with `oneBit::Mask<>` for hierarchical port control:

```cpp
#include <oneBit/oneBit.h>
using namespace oneBit;

// SPI pins: MOSI(3), MISO(4), SCK(5)
using SpiPins = Mask<Pins<3, 4, 5>>;
using SpiPort = AVR::OutPin<SpiPins, chip::PortB>;

SpiPort spi;
spi.dir(SpiPins::value);   // Configure all 3 pins as outputs
spi.put(0b00111000);       // Write all SPI pins at once
```

---

## Integration with OneChip

OnePin provides the terminal layer for OneChip hardware:

```cpp
#include <chips/avr/avrDevice.h>

using Led = AVR::OutPin<Pins<5>, chip::PortB>;
using Btn = AVR::InPin<Pins<0>, chip::PortB>;

// Now integrate with OneChip timers, interrupts, etc.
using Board = AVR::Board<Boot<SysTick>, Led>;
```

---

## Common Patterns

### Simple LED Blink
```cpp
using Led = AVR::OutPin<Pins<5>, chip::PortB>;
Led led;
led.dir(1<<5);
for(;;) {
  led.on();
  delay(100);
  led.off();
  delay(900);
}
```

### Multi-Pin Bus
```cpp
using Bus = AVR::OutPin<Pins<0,1,2,3>, chip::PortB>;
Bus bus;
bus.dir(0x0F);        // Set PB0..PB3 as outputs
bus.put(0b1010);      // Write pattern
uint8_t state = bus.get();  // Read current
```

### Bidirectional Pin (Open-Drain)
```cpp
using I2cSda = AVR::IOPin<Pins<4>, chip::PortB>;
I2cSda sda;
sda.dir(0);           // Release (high-impedance)
sda.off();            // Pull low
uint8_t level = sda.get();  // Read bus level
```

---

## See Also

- [OneChip](../../../OneChip/docs/REFERENCE.md) — Hardware platforms & interrupts
- [OneBit](../../../OneBit/docs/REFERENCE.md) — Bit masking utilities
- [OneInput](../../../OneInput/docs/REFERENCE.md) — Input drivers & filtering
