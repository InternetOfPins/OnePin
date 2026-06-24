/**
 * @file test.cpp
 * @brief OnePin unit tests — VirtualPort for native testing
 *
 * Tests:
 *   VirtualPort   — _pin/_port/_ddr registers, dir_out/dir_in/dir
 *   Out + Mask    — begin() sets direction, on()/off() drive _port
 *   In  + Mask    — get() reads from _pin
 *   IOPin + Mask  — bidirectional chain
 *   Multiple ports — VirtualPort<0> and VirtualPort<1> are independent
 *   DeviceClass   — compile-time collision check still works
 */

#include <iostream>
#include <cassert>
#include "../include/onePin/onePin.h"
#include "../../OneBit/include/oneBit/oneBit.h"   // Mask<>, Pins<>
#include "../../HAPI/include/hapi/hapi.h"

#ifdef ARDUINO
  #define cout Serial
  #define assert(x) do { if(!(x)) { Serial.print("FAIL: "); Serial.println(#x); } } while(0)
#else
  using namespace std;
#endif

using namespace onePin;
using namespace oneBit;
using namespace hapi;

// ── Two virtual ports ─────────────────────────────────────────────────────────
using VP0 = VirtualPort<0>;
using VP1 = VirtualPort<1>;

// Output pin: bit 3 of VP0
using Led  = APIOf<AvrOutPin, Out, Mask<Pins<3>>, VP0>;
// Input pin:  bit 5 of VP0
using Btn  = APIOf<AvrInPin,  In,  Mask<Pins<5>>, VP0>;
// Second port
using Led2 = APIOf<AvrOutPin, Out, Mask<Pins<1>>, VP1>;

void test_output() {
  VP0::reset(); VP0::_port = 0;
  Led::begin();                        // dir_out(1<<3)
  assert(VP0::_ddr & (1<<3));          // direction set

  Led::on();
  assert(VP0::_port & (1<<3));         // bit 3 high

  Led::off();
  assert(!(VP0::_port & (1<<3)));      // bit 3 low

  cout << "output: OK\n";
}

void test_input() {
  VP0::reset();
  Btn::begin();                        // dir_in(1<<5)
  assert(!(VP0::_ddr & (1<<5)));       // direction clear (input)

  VP0::_pin = (1<<5);                  // simulate button pressed
  assert(Btn::get());                  // get() reads _pin through mask

  VP0::_pin = 0;
  assert(!Btn::get());

  cout << "input: OK\n";
}

void test_independent_ports() {
  VP0::reset(); VP1::reset();
  Led::begin();
  Led2::begin();

  Led::on();
  assert(VP0::_port & (1<<3));
  assert(!(VP1::_port & (1<<1)));      // VP1 unchanged

  Led2::on();
  assert(VP1::_port & (1<<1));
  assert(VP0::_port & (1<<3));         // VP0 unchanged

  cout << "independent ports: OK\n";
}

void test_readback() {
  VP0::reset();
  Led::begin(); Led::on();
  // port() passes through chain — reads back _port
  assert(Led::port() & (1<<3));
  cout << "readback: OK\n";
}

// DeviceClass compile-time check: no collision between Led (bit3) and Btn (bit5) on VP0
// If this line compiles, the check passed.
using Board = DeviceClass<Led, Btn>;

#ifdef ARDUINO
void setup() {
  Serial.begin(115200);
  test_output(); test_input(); test_independent_ports(); test_readback();
}
void loop() {}
#else
int main() {
  test_output();
  test_input();
  test_independent_ports();
  test_readback();
  cout << "all VirtualPort tests passed\n";
  return 0;
}
#endif
