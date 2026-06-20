/**
 * @file onePin.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief OnePin — pin/port API terminals and compile-time device allocation.
 *        Hardware implementations (AVRPort, STM32Port) are provided by OneChip.
 *        Bit/mask selection is provided by OneBit (Mask<Pins<N>>, Mask<MaskVal<M>>).
 *
 * Layer model (outermost → innermost):
 *   Terminal (InPin/OutPin/IOPin)
 *   oneBit::Mask<Pins<N>>      ← bakes mask into on()/off()/get(); uses port()/pin()
 *   HwPort (AVRPort/STM32Port) ← pin() input, port()/port(v) output latch r/w
 *
 * API contract:
 *   pin()         — input state  (PIN register AVR / IDR STM32)
 *   port()        — output latch (PORT register AVR / ODR STM32), passes through Mask<>
 *   port(Unit v)  — whole-port write, unmasked, passes through Mask<>
 *   get()         — masked input read  (provided by Mask<>)
 *   set(Unit v)   — masked output write (provided by Mask<>)
 *   on() / off()  — maskless pin set/clear (provided by Mask<>)
 *   dir(Unit m)   — port direction (provided by HwPort)
 */

#pragma once
#include <hapi/hapi.h>
#include <OneBit.h>

namespace onePin {
  using namespace hapi;

  // ============================================================
  // Terminals — silent stubs; hardware components override below
  // ============================================================

  template<typename U = unsigned char>
  struct InPin {
    using Unit = U;
    using Type = U;
    static void begin()     {}
    static Unit pin()       { return 0; }
    static Unit port()      { return 0; }
    static void port(Unit)  {}
    static Unit ddr()       { return 0; }
    static void dir(Unit)   {}
    static void act()       {}
  };

  template<typename U = unsigned char>
  struct OutPin {
    using Unit = U;
    using Type = U;
    static void begin()     {}
    static Unit pin()       { return 0; }
    static Unit port()      { return 0; }
    static void port(Unit)  {}
    static Unit ddr()       { return 0; }
    static void dir(Unit)   {}
    static void on()        {}
    static void off()       {}
    static void act()       {}
  };

  template<typename U = unsigned char>
  struct IOPin {
    using Unit = U;
    using Type = U;
    static void begin()     {}
    static Unit pin()       { return 0; }
    static Unit port()      { return 0; }
    static void port(Unit)  {}
    static Unit ddr()       { return 0; }
    static void dir(Unit)   {}
    static void dir_out()   {}
    static void dir_in()    {}
    static void on()        {}
    static void off()       {}
    static void act()       {}
  };

  // ============================================================
  // Out / In — direction-setter components.
  // Place above Mask<> in the chain; begin() calls dir_out()/dir_in() automatically.
  //
  //   using Led = APIOf<AvrOutPin, Out, Mask<Pins<5>>, chip::PortB>;
  //   Led::begin();   // dir_out() called — no manual led.dir_out() needed
  // ============================================================
  struct Out {
    template<typename O>
    struct Part : O {
      using O::O;
      static void begin() { O::dir_out(); O::begin(); }
    };
  };

  struct In {
    template<typename O>
    struct Part : O {
      using O::O;
      static void begin() { O::dir_in(); O::begin(); }
    };
  };

  // ============================================================
  // detail — HAPI-idiom helpers for DeviceClass
  // ============================================================
  namespace detail {

    // HAPI predicate: matches any type exposing 'allowedMask' (port types)
    struct IsPort {
      template<typename T, typename = void>
      struct Check : std::false_type {};
      template<typename T>
      struct Check<T, std::void_t<decltype(T::allowedMask)>> : std::true_type {};
    };

    // Map-based count: MatchToIC<P> maps each element to integral_constant<int,0|1>
    // Chain::Build<SumIC> then folds them. Map recurses into nested chains automatically.
    template<typename P>
    struct MatchToIC {
      template<typename O>
      struct Apply {
        using Expr = std::integral_constant<int, P::template Check<O>::value ? 1 : 0>;
      };
    };

    // Sum of integral_constants (flat result of Map over a non-nested chain)
    template<typename... ICs>
    struct SumIC { static constexpr int value = (ICs::value + ...); };

    template<typename P, typename C>
    static constexpr int countByMap = hapi::Map<MatchToIC<P>, C>::Expr::template Build<SumIC>::value;

    // firstInChain<P>(Chain<...>) — return first matching element instance (for decltype)
    // Sequential by nature; Map-based extraction is future work.
    template<typename P>
    constexpr hapi::Nil firstInChain(hapi::Chain<>) { return {}; }

    template<typename P, typename... Inner, typename... Rest>
    constexpr auto firstInChain(hapi::Chain<hapi::Chain<Inner...>, Rest...>) {
      using inner_t = decltype(firstInChain<P>(hapi::Chain<Inner...>{}));
      if constexpr (!std::is_same_v<inner_t, hapi::Nil>)
        return firstInChain<P>(hapi::Chain<Inner...>{});
      else
        return firstInChain<P>(hapi::Chain<Rest...>{});
    }

    template<typename P, typename H, typename... Rest>
    constexpr auto firstInChain(hapi::Chain<H, Rest...>) {
      if constexpr (P::template Check<H>::value)
        return H{};
      else
        return firstInChain<P>(hapi::Chain<Rest...>{});
    }

    // has_begin_fn — detects static begin() method
    template<typename T, typename = void>
    struct has_begin_fn : std::false_type {};
    template<typename T>
    struct has_begin_fn<T, std::void_t<decltype(T::begin())>> : std::true_type {};

    template<typename T>
    static void begin_if() {
      if constexpr (has_begin_fn<T>::value) T::begin();
    }

  } // namespace detail

  // ============================================================
  // PortAlloc<Port, PinChains...> — single-port collision check
  // ============================================================
  template<typename Port, typename... PinChains>
  struct PortAlloc {
    using Unit = typename Port::Unit;

    template<typename PC>
    static constexpr Unit maskOf =
      oneBit::extractMask<Unit>(typename PC::Types::Tail{});

    static constexpr bool inBounds() {
      return (((maskOf<PinChains> & Port::allowedMask) == maskOf<PinChains>) && ...);
    }

    template<Unit... Masks>
    static constexpr bool checkOverlap() {
      Unit acc = 0; bool ok = true;
      ((ok = ok && !(Masks & acc), acc |= Masks), ...);
      return ok;
    }

    static_assert(inBounds(),
      "PortAlloc: pin mask outside port allowedMask");
    static_assert(checkOverlap<maskOf<PinChains>...>(),
      "PortAlloc: pin collision — two pins share a bit on the same port");
  };

  // ============================================================
  // DeviceClass<Peripherals...> — device-wide allocation check
  //
  // Rules enforced at compile time:
  //   1. Each peripheral has exactly one Mask<> in its chain
  //   2. No two peripherals share a bit on the same port
  //
  // Uses HAPI predicates:
  //   IsInstanceOf<oneBit::Mask>  — identifies Mask<MaskDesc> in chain
  //   detail::IsPort              — identifies HwPort by presence of allowedMask
  //
  // Future: Periph<PinChains...> will group multi-pin peripherals (SPI, I2C, UART)
  // so the block-level check applies at peripheral granularity.
  // ============================================================
  template<typename... Peripherals>
  struct DeviceClass {
  private:
    // Port type of a peripheral: first element in Types::Tail matching IsPort
    template<typename PC>
    using port_t_ = decltype(detail::firstInChain<detail::IsPort>(
      typename PC::Types::Tail{}));

    // Mask value for a peripheral (via existing extractMask constexpr walker)
    template<typename PC>
    static constexpr typename port_t_<PC>::Unit maskOf =
      oneBit::extractMask<typename port_t_<PC>::Unit>(typename PC::Types::Tail{});

    // Count of Mask<> components in a peripheral chain — via Map<MatchToIC> + Build<SumIC>
    template<typename PC>
    static constexpr int maskCount =
      detail::countByMap<hapi::IsInstanceOf<oneBit::Mask>, typename PC::Types::Tail>;

    // Pairwise same-port no-conflict check
    template<typename P1, typename P2>
    static constexpr bool noConflict() {
      if constexpr (std::is_same_v<port_t_<P1>, port_t_<P2>>)
        return !(maskOf<P1> & maskOf<P2>);
      return true;
    }

    template<typename P, typename... Rest>
    static constexpr bool checkAgainstAll() {
      if constexpr (sizeof...(Rest) == 0) return true;
      return (noConflict<P, Rest>() && ...);
    }

    template<typename...>
    struct PairwiseCheck { static constexpr bool value = true; };
    template<typename P, typename... Rest>
    struct PairwiseCheck<P, Rest...> {
      static constexpr bool value =
        checkAgainstAll<P, Rest...>() && PairwiseCheck<Rest...>::value;
    };

  public:
    static_assert(((maskCount<Peripherals> == 1) && ...),
      "DeviceClass: each peripheral must have exactly one Mask<> in its chain");

    static_assert(((( maskOf<Peripherals> & port_t_<Peripherals>::allowedMask)
                      == maskOf<Peripherals>) && ...),
      "DeviceClass: pin mask outside port allowedMask");

    static_assert(PairwiseCheck<Peripherals...>::value,
      "DeviceClass: pin collision — two peripherals share a bit on the same port");
  };

  // ============================================================
  // BootDef — terminal for Boot item chains (same role as InPin/OutPin for GPIO)
  // Provides begin() eraser; hardware boot components override via Part<O>.
  // ============================================================
  struct BootDef {
    static void begin() {}
  };

  // ============================================================
  // Boot<Items...> — init-first section (clocks, timers, system resources)
  // Place at head of Device<> so its items run before GPIO peripherals.
  // ============================================================
  template<typename... Items>
  struct Boot {};

  // ============================================================
  // Device<Boot<...>, Peripherals...>
  // Enforces DeviceClass rules on Peripherals, then on begin():
  //   1. Boot items initialised first (in order)
  //   2. GPIO peripherals initialised after
  // Only calls begin() on items that have it (has_begin_fn).
  // ============================================================
  template<typename... All>
  struct Device;

  template<typename... BootItems, typename... Peripherals>
  struct Device<Boot<BootItems...>, Peripherals...> : DeviceClass<Peripherals...> {
    static void begin() {
      (BootItems::begin(), ...);
      (Peripherals::begin(), ...);
    }

    template<typename Fn>
    static void run(Fn fn) { for (;;) fn(); }
  };

  // ============================================================
  // Convenience aliases
  // ============================================================
  using AvrInPin  = InPin<unsigned char>;
  using AvrOutPin = OutPin<unsigned char>;
  using AvrIOPin  = IOPin<unsigned char>;

  using Stm32InPin  = InPin<unsigned int>;
  using Stm32OutPin = OutPin<unsigned int>;
  using Stm32IOPin  = IOPin<unsigned int>;

} // namespace onePin

// ============================================================
// Usage:
//
//   #include <onePin/onePin.h>
//   #include <chips/avr/avrPort.h>
//   #include <OneBit.h>
//   using namespace onePin; using namespace hw::avr; using namespace oneBit;
//
//   using Led = APIOf<AvrOutPin, Mask<Pins<5>>, chip::PortB>;
//   using Btn = APIOf<AvrInPin,  Mask<Pins<3>>, chip::PortB>;
//
//   using Board = DeviceClass<Led, Btn>;   // compile-time check: ok
//   // DeviceClass<Led, Led>               // error: pin collision
//   // DeviceClass<AvrOutPin>              // error: no Mask<> in chain
//
//   Led led; led.on();       // PB5 high (via Mask<>)
//            led.off();
//            led.port(0xFF); // raw whole-port write (bypasses Mask<>)
//   Btn btn; auto v = btn.get();  // masked input read (bit 3 only)
// ============================================================
