/**
 * @file onChange.h
 * @brief OnChange/OnRise/OnFall — interrupt source abstraction.
 *
 * Duck-type concept for platform-agnostic edge detection across AVR/ESP32/STM32.
 * Allows encoders and input drivers to work identically on all platforms.
 *
 * Contract: Any type T satisfying is_change_source<T> provides:
 *   template<typename EdgeMode> static void begin()
 *   template<typename EdgeMode> static uint8_t read()
 *   template<typename EdgeMode> static bool changed()
 *
 * Usage:
 *   using MyInt = chip::OnChange<A0, A1>;  // resolves to platform impl
 *   MyInt::begin();
 *   uint8_t state = MyInt::read();
 *   if (MyInt::changed()) { handle_edge(); }
 */

#pragma once
#ifdef __AVR__
  #include <hapi/platform/avr/avr_std.h>
#else
  #include <type_traits>
#endif
#include <stdint.h>

namespace onePin {

  // Edge mode markers (no members; just tag types) — defined first for SFINAE
  struct OnChange {};  ///< Any edge: rising or falling
  struct OnRise {};    ///< Rising edge only
  struct OnFall {};    ///< Falling edge only

  /// SFINAE trait: does T satisfy is_change_source contract?
  /// Checks for non-templatized begin(), read(), changed() methods
  template<typename T, typename = void>
  struct is_change_source : std::false_type {};

  template<typename T>
  struct is_change_source<T, std::void_t<
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().read()),
    decltype(std::declval<T>().changed())
  >> : std::true_type {};

  /// Silent terminal: satisfies is_change_source, useful for testing
  struct ChangeSourceTerm {
    static void begin() {}
    static uint8_t read() { return 0; }
    static bool changed() { return false; }
  };

  static_assert(is_change_source<ChangeSourceTerm>::value,
    "ChangeSourceTerm must satisfy is_change_source");

} // onePin

// Forward declare OnChange/OnRise/OnFall so they can be used in SFINAE above
// (they're defined in chip namespace, but referenced here for trait detection)
