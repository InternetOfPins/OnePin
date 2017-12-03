# OnePin


Pin level meta-framework

## Purpose

1. Provide zero cost abstraction to multiple pin frameworks.
2. Abstract common concepts as logic level inversion.
3. Provide pin composition on client side. Allows pin based libraries to abstract of pin details.
4. Base for higher abstractions as pin routing and IoP (Internet of Pins)

## Technique

Using c++ type level programming to overlay existing frameworks with zero-cost.

## Examples

### Arduino framework blinking

```c++
#include <OneArduino.h>

using namespace OneLib;
using namespace OneLib::Arduino;

typedef OutputPin<LED_BUILTIN> Led;

void setup() {
  Led::begin();
}

void loop() {
  Led::tog();
  delay(500);
}
```

### Raw AVR blinking


```c++
#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;

typedef OutputPin<PortB,5> Led;

void setup() {
  Led::begin();
}

void loop() {
  Led::tog();
  _delay_ms(200);
}
```

### Mixed code

Using Raw AVR pin access and arduino higher functions like `millis`

```c++
#include <OneAvr.h>

using namespace OneLib;
using namespace OneLib::Avr;
using namespace OneLib::Avr::AtMega328p;

template<unsigned int on,unsigned int off>
inline bool tog() {return (millis()%(on+off))<on;}

//static hardware description
typedef OutputPin<PortB,5> Led1;//pin 13 on arduino
typedef OutputPin<PortC,3> Led2;//pin A3 on arduino

void setup() {
  Led1::begin();
  Led2::begin();
}

void loop() {
  Led1::set(tog<10,90>());
  Led2::set(tog<500,500>());
}
```

## Benchmark

**Operations by time unit (higher is better)**

Control and Empty functions are only test calibration stages where no operation is performed. As seen on control stage test slots are not homogeneous, so tests are done outside that area, hence the use of `Empty function`

### Control
| Test                          |#Ops  |Debounced|OnePin   |
|------------------------------:|------:|------:|------:|
|Empty function:                |3976   |4071   |4071   |
|Empty function:                |4040   |4070   |4071   |
|Empty function:                |4041   |4071   |4070   |
|Empty function:                |4040   |4071   |4071   |
### pinMode
| Test                          |#Ops  |Debounced|OnePin   |
|------------------------------:|------:|------:|------:|
|Empty function:                |3976   |4070   |4071   |
|Arduino pin mode:              |2188   |4071   |2205   |
|OneLib Arduino mode:           |2185   |2205   |2206   |
|OneLib AVR mode:               |3352   |3377   |3377   |
|OneLib VoidPin mode:           |4033   |4071   |4070   |
### digitalRead
| Test                          |#Ops  |Debounced|OnePin   |
|------------------------------:|------:|------:|------:|
|Empty function:                |3977   |4070   |4070   |
|Arduino pin input:             |2404   |4071   |2423   |
|OneLib Arduino input:          |2184   |1095   |2206   |
|OnePin AVR input:              |4037   |1417   |4070   |
|OneLib VoidPin input:          |4032   |1491   |4071   |
### digitalWrite
| Test                          |\#Ops  |Debounced|OnePin   |
|------------------------------:|------:|------:|------:|
|Empty function:                |3977   |4071   |4070   |
|Arduino pin output:            |2216   |4071   |2236   |
|OneLib Arduino output:         |2215   |2236   |2236   |
|OnePin AVR output:             |3747   |3780   |3780   |
|OneLib VoidPin output:         |4031   |4070   |4070   |
