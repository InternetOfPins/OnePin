/**
 * @file onePin.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief OnePin — generic pin/port abstraction API for HAPI chains.
 *        Hardware implementations are provided by OneChip.
 */

#pragma once
#include <cstdint>
#include <hapi/hapi.h>

namespace onePin {
  using namespace hapi;
  using Unit = uint8_t;

  // ====================== Pin API terminal ======================
  // Fallback implementation — silently does nothing.
  // Place at the bottom of the chain; hardware components layer on top.

  struct PinAPI {
    static void set(Unit) {}
    static Unit get()     { return 0; }
    static void dir(Unit) {}
  };

} // namespace onePin
