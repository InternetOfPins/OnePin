/**
 * @file onePin.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief OnePin — generic pin/port abstraction API for HAPI chains.
 *        Hardware implementations live in OneChip.
 */

#pragma once
#include <cstdint>
#include <hapi/hapi.h>

namespace onePin {
  using namespace hapi;
  using Addr = uintptr_t;
  using Unit = uint8_t;

  // ====================== Pin API ======================

  struct PinAPI {
    template<typename O>
    struct Part : O {
      using O::O;
      static void set(Unit) {}
      static Unit get()     { return 0; }
      static void dir(Unit) {}
    };
  };

  // ====================== AVR port components ======================
  // Hardware register addresses as template parameters.
  // Will migrate to OneChip when that library matures.

  template<Addr pIn>
  struct AVRPortRead {
    template<typename O>
    struct Part : O {
      using Base = O;
      static volatile Unit get() { return *reinterpret_cast<volatile Unit*>(pIn); }
    };
  };

  template<Addr ddr>
  struct AVRPortDir {
    template<typename O>
    struct Part : O {
      using Base = O;
      static void dir(Unit bits) { *reinterpret_cast<volatile Unit*>(ddr) = bits; }
    };
  };

  template<Addr port>
  struct AVRPortWrite {
    template<typename O>
    struct Part : O {
      using Base = O;
      static void set(Unit val) { *reinterpret_cast<volatile Unit*>(port) = val; }
    };
  };

  // AVRPort<pIn, ddr, port>: composes read/dir/write into one port component.
  // ATmega AVR memory map: PIN=base, DDR=base+1, PORT=base+2.
  template<Addr pIn, Addr ddr = pIn+1, Addr port = pIn+2>
  struct AVRPort {
    using Chain_ = Chain<AVRPortRead<pIn>, AVRPortDir<ddr>, AVRPortWrite<port>>;
    template<typename O>
    struct Part : Chain_::template Part<O> {
      using Base = typename Chain_::template Part<O>;
      using Base::Base;
    };
  };

  // ====================== ATmega328P port addresses ======================
  static constexpr Addr portAddr_B = 0x23;
  static constexpr Addr portAddr_C = 0x26;
  static constexpr Addr portAddr_D = 0x29;

  using PortB = AVRPort<portAddr_B>;
  using PortC = AVRPort<portAddr_C>;
  using PortD = AVRPort<portAddr_D>;

} // namespace onePin
