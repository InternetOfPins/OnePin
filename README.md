# OnePin


**Pin level meta-framework**

## What it is not.

This is not a framework and therefor not a candidate to replace any framework.
Meaning we require an existing one.

## Purpose

This is the logical complement of the internet of things, for this whole thing called internet came to the `thing`, usually in the form of an MCU. This thing that characterizes this `Things` on the internet its the ability to interact with the world, usually in the form of pins to control parts or read sensors.

The InternetOfPins is to provide this last complement and fill the gap between the so called `thing` and its pins, and pins of pins and so on, opening a branch of interconnection between devices and signals to the external world as well as for benefits of the interconnected `things`

## Commitment

Do this approach not breaking with existing technologies and the vastness of priceless code, but using it on the most transparent and light way possible.

Provide some uniformity for the sake of library development, allowing new developed code to have a wider range of usage.

Use the new C++ features that allow or ease some tasks, but stay on a good compatibility level with most frameworks (C++11 for now)

Do NOT sacrifice performance in vane.

## Motivation

Allow library writers to be more generic and target multiple frameworks at once.
Because current library development requires changing the library code in order to:

- Support software debounce
- Use another framework
- Support same hardware connected over I²C, SPI or any other expansion port (⁺pin routing and virtual pins proposals).
- Reverse logic, some libraries make an internal management of pin normal state. We need not.
- Signal NA pins by using VoidPin, this abstraction removes pin output code.

Enrich the embedded development with some extra connectivity, by providing abstraction of external pins but also allowing devices to publish direct or indirect owned pins, providing a flow path for pin information/signals.

## Goals

Touch this key areas to prove its feasibility.

1. Provide zero cost abstraction to multiple pin frameworks, as required base to efficiently achieve the following points. ✔
2. Abstract common concepts as logic level inversion. ✔
3. Provide pin composition on client side. Allows pin based libraries to abstract of pin details. ✔
4. Base for higher abstractions as pin routing and IoP (Internet of Pins) ✔
5. Interface with externally compiled libraries on a non-type level ✔
6. Non embedded version of IoP.

## Technique

Using c++ type level programming to overlay existing frameworks with zero-cost.

By zero-cost we mean: the abstraction will either be absent or represent the functionality.

**Zero-cost** meta-programming with static inline functions.

**Composition** using c++ mixins.

**OnePin** top level pin class with virtual functions to cover low level mixins.
This class allows delivering a generic pin, breaking the template/mixin chain.
_Libraries should use this type of pin_

### Current state

This is an ongoing work on a preliminary phase, so expect this documentation to be lagging.

We are touching key aspects of the problem and proving its feasibility.
Latter on this areas can be fine tunned and enlarged to support more cases.

#### Zero cost-overlay.
This is a type level overlay of pin manipulation, allowing set/read pin state and eventually setting pin mode.
Because its a type level definition all deduction occurs at compile time, resulting on the direct call of underlining framework.

It is possible fully manipulate the pins, setting modes, reading or write data from the type level.

**example:**

Define the type of the pin.
```c++
typedef PinCap<OutputPin<13>> Led;
```
Initialize the pin, on some platforms/pin types its equivalent to set the pin mode, but on some hardware it generates no code.
```c++
Led::begin();
```
Setting the pin state
```c++
Led::on();
```

#### Abstract concepts as pin composition modules
Some ubiquitous abstractions as logic level inversion are built in and available as a pin composition.
Because of its nature, it provides a simple exclusive-or bit inversions when needed or transparently do the underling call, they are automatically included on every pin type.
However it serves an an example because some other abstraction can and are provided this way, as pin composition modules.
This composition is done on the program side and libraries need not know the details but all can use the pin.
From the library of program side the pin has its uniform PinCap interface.
**This is still at type level** see Features for a list on implemented compositions

#### IoP
First preliminary module available, we can now use shift-register's pins wired over hardware SPI and use the pins to output data.

Still we need to extend this to SPI Input and add some extra buses like I²C.

This is only implemented for Arduino framework, still need to add more frameworks.

#### Non-type level library interface
Because libraries need not be compiled to match type level pin definition, a runtime overlay is provided.
This only serves libraries coded for this generic overlay however.

Previous approaches to this problem by patching existing frameworks was comprehensibly not accepted enthusiastically, because its a patch and would involve a performance burden, even if minimal. This approach would however make existing libraries use this routing capabilities with no required changes.

The good part is that the approach is still available and does not collide with OnePin implementation, but can now benefit from it.

### Features

#### PinCap
Provide an uniform pin top layer at type level.


#### VoidPin

Not only ignores operation as it removes the pin-output code, input code is resumed to false.

#### Software debounce to a pin

This is also a zero cost abstraction, meaning that the composition is code equivalent to doing that software debounce on the client function.

#### Reverse logic

Check for negative pin number and overlays a reverse logic.

It is composed by OneLib::OnePin to provide logic invertion when a negative pin or bit position is given.

This composition is transparent on non-invertible pins (no extra code generated).

#### Pin-change action

Attaches a function to be called whenever the pin changes

* OnRise - changed to up state
* OnFall - changed to down state
* OnChange - both directions

#### State record

* RecState - updates the last state of a pin after read, this must be the top mixin (before PinCap)

Used by **soft-debunce** and **pin-change actions**.

### Pin composition examples

```c++
//soft debounced AVR pin
typedef RecState<Debouncer<Avr::Pin<PortB,5>,30>> Deb13;
//soft debounced Arduino pin
typedef RecState<Debouncer<Arduino::Pin<13>>> Pin13;
```

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

### Virtual pins

Using shift register pins over Arduino's hardware SPI port.

```c++
#include <OneArduino.h>
#include <OneArduino/VPinSPI.h>

using namespace OneLib;
using namespace OneLib::Arduino;

//use pin 9 as latch (transfer to output)
typedef PinCap<OutputPin<9>> LatchPin;
//describe SPI shift registers geometry
typedef SPIExt<SPI, LatchPin,1> SPI_8;//a set of 8 output pins

//allocate output pins from SPI shift registers SPI_8, bit 0
typedef PinCap<SPIPin<SPI_8,0>> SPILed;

void setup() {
  //will init SPI and latch pin
  SPI_8::begin();
}

void loop() {
  SPILed::on();
  delay(100);
  SPILed::off();
  delay(900);
}

```

## Generated code

_this data can be outdated_

**Arduino BtnBlink**

>Program:    1154 bytes (3.5% Full)

>Data:          9 bytes (0.4% Full)

**OneLib BtnBlink**

>Program:     702 bytes (2.1% Full)

>Data:          9 bytes (0.4% Full)

## Benchmark

**Operations by time unit (higher is better)**

Control and Empty functions are only test calibration stages where no operation is performed. As seen on control stage test slots are not homogeneous, so tests are done outside that area, hence the use of `Empty function`

### Control
| Test                          |Empty  |#Ops   |Deb.   |OnePin |
|------------------------------:|------:|------:|------:|------:|
|Empty function:                |4190   |4291   |4291   |4289   |
|Empty function:                |4258   |4289   |4290   |4289   |
|Empty function:                |4257   |4289   |4290   |4290   |
|Empty function:                |4258   |4289   |4290   |4290   |
|Empty function:                |4259   |4289   |4289   |4290   |
### pinMode
| Test                          |Empty  |#Ops   |Deb.   |OnePin |
|------------------------------:|------:|------:|------:|------:|
|Empty function:                |-      |-      |-      |-      |
|Arduino pin mode:              |-      |2208   |-      |-      |
|OneLib Arduino mode:           |-      |2162   |2161   |2160   |
|OneLib AVR mode:               |-      |3381   |3378   |3378   |
|OneLib VoidPin mode:           |-      |4294   |-      |4295   |
### digitalRead
| Test                          |Empty  |#Ops   |Deb.   |OnePin |
|------------------------------:|------:|------:|------:|------:|
|Empty function:                |-      |-      |-      |-      |
|Arduino pin input:             |-      |2427   |-      |-      |
|OneLib Arduino input:          |-      |2207   |2690   |2206   |
|OnePin AVR input:              |-      |4294   |2691   |4291   |
|OneLib VoidPin input:          |-      |4294   |-      |4295   |
### digitalWrite
| Test                          |Empty  |#Ops   |Deb.   |OnePin |
|------------------------------:|------:|------:|------:|------:|
|Empty function:                |-      |-      |-      |-      |
|Arduino pin output:            |-      |2239   |-      |-      |
|OneLib Arduino output:         |-      |2192   |2160   |2190   |
|OnePin AVR output:             |-      |3784   |3691   |3780   |
|OneLib VoidPin output:         |-      |4294   |-      |4295   |


## Disassembly compare

comparing part of the main function from the example above with the equivalent arduino code.

Target: AtMega328P (current is not a fair compare because OneLib is targeting the raw AVR)

#### blinking with OnePin
```assembly
272:	85 b1       	in	r24, 0x05	; 5
274:	80 62       	ori	r24, 0x20	; 32
276:	4c 99       	sbic	0x09, 4	; 9
278:	0d c0       	rjmp	.+26     	; 0x294 <main+0xb2>
27a:	85 b9       	out	0x05, r24	; 5
27c:	6a e0       	ldi	r22, 0x0A	; 10
27e:	70 e0       	ldi	r23, 0x00	; 0
280:	80 e0       	ldi	r24, 0x00	; 0
282:	90 e0       	ldi	r25, 0x00	; 0
284:	0e 94 6d 00 	call	0xda	; 0xda <delay>
288:	2d 98       	cbi	0x05, 5	; 5
28a:	6a e5       	ldi	r22, 0x5A	; 90
28c:	70 e0       	ldi	r23, 0x00	; 0
28e:	80 e0       	ldi	r24, 0x00	; 0
290:	90 e0       	ldi	r25, 0x00	; 0
292:	0c c0       	rjmp	.+24     	; 0x2ac <main+0xca>
294:	85 b9       	out	0x05, r24	; 5
296:	64 e6       	ldi	r22, 0x64	; 100
298:	70 e0       	ldi	r23, 0x00	; 0
29a:	80 e0       	ldi	r24, 0x00	; 0
29c:	90 e0       	ldi	r25, 0x00	; 0
29e:	0e 94 6d 00 	call	0xda	; 0xda <delay>
2a2:	2d 98       	cbi	0x05, 5	; 5
2a4:	64 e6       	ldi	r22, 0x64	; 100
2a6:	70 e0       	ldi	r23, 0x00	; 0
2a8:	80 e0       	ldi	r24, 0x00	; 0
2aa:	90 e0       	ldi	r25, 0x00	; 0
2ac:	0e 94 6d 00 	call	0xda	; 0xda <delay>
2b0:	20 97       	sbiw	r28, 0x00	; 0
2b2:	f9 f2       	breq	.-66     	; 0x272 <main+0x90>
2b4:	0e 94 00 00 	call	0	; 0x0 <__vectors>
2b8:	dc cf       	rjmp	.-72     	; 0x272 <main+0x90>
```

#### blinking with Arduino framework
```assembly
3fa:	f7 01       	movw	r30, r14
3fc:	84 91       	lpm	r24, Z
3fe:	fe 01       	movw	r30, r28
400:	a4 90       	lpm	r10, Z
402:	f8 01       	movw	r30, r16
404:	b4 90       	lpm	r11, Z
406:	bb 20       	and	r11, r11
408:	09 f1       	breq	.+66     	; 0x44c <main+0x164>
40a:	81 11       	cpse	r24, r1
40c:	0e 94 75 00 	call	0xea	; 0xea <turnOffPWM>
410:	eb 2d       	mov	r30, r11
412:	f0 e0       	ldi	r31, 0x00	; 0
414:	ee 0f       	add	r30, r30
416:	ff 1f       	adc	r31, r31
418:	cf 01       	movw	r24, r30
41a:	8e 58       	subi	r24, 0x8E	; 142
41c:	9f 4f       	sbci	r25, 0xFF	; 255
41e:	fc 01       	movw	r30, r24
420:	a5 91       	lpm	r26, Z+
422:	b4 91       	lpm	r27, Z
424:	8c 91       	ld	r24, X
426:	a8 22       	and	r10, r24
428:	89 f0       	breq	.+34     	; 0x44c <main+0x164>
42a:	81 e0       	ldi	r24, 0x01	; 1
42c:	0e 94 9e 00 	call	0x13c	; 0x13c <digitalWrite.constprop.1>
430:	64 e6       	ldi	r22, 0x64	; 100
432:	70 e0       	ldi	r23, 0x00	; 0
434:	80 e0       	ldi	r24, 0x00	; 0
436:	90 e0       	ldi	r25, 0x00	; 0
438:	0e 94 f0 00 	call	0x1e0	; 0x1e0 <delay>
43c:	80 e0       	ldi	r24, 0x00	; 0
43e:	0e 94 9e 00 	call	0x13c	; 0x13c <digitalWrite.constprop.1>
442:	64 e6       	ldi	r22, 0x64	; 100
444:	70 e0       	ldi	r23, 0x00	; 0
446:	80 e0       	ldi	r24, 0x00	; 0
448:	90 e0       	ldi	r25, 0x00	; 0
44a:	10 c0       	rjmp	.+32     	; 0x46c <main+0x184>
44c:	81 e0       	ldi	r24, 0x01	; 1
44e:	0e 94 9e 00 	call	0x13c	; 0x13c <digitalWrite.constprop.1>
452:	6a e0       	ldi	r22, 0x0A	; 10
454:	70 e0       	ldi	r23, 0x00	; 0
456:	80 e0       	ldi	r24, 0x00	; 0
458:	90 e0       	ldi	r25, 0x00	; 0
45a:	0e 94 f0 00 	call	0x1e0	; 0x1e0 <delay>
45e:	80 e0       	ldi	r24, 0x00	; 0
460:	0e 94 9e 00 	call	0x13c	; 0x13c <digitalWrite.constprop.1>
464:	6a e5       	ldi	r22, 0x5A	; 90
466:	70 e0       	ldi	r23, 0x00	; 0
468:	80 e0       	ldi	r24, 0x00	; 0
46a:	90 e0       	ldi	r25, 0x00	; 0
46c:	0e 94 f0 00 	call	0x1e0	; 0x1e0 <delay>
470:	c1 14       	cp	r12, r1
472:	d1 04       	cpc	r13, r1
474:	09 f4       	brne	.+2      	; 0x478 <main+0x190>
476:	c1 cf       	rjmp	.-126    	; 0x3fa <main+0x112>
478:	0e 94 00 00 	call	0	; 0x0 <__vectors>
47c:	be cf       	rjmp	.-132    	; 0x3fa <main+0x112>
```
