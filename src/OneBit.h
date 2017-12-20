/* -*- C++ -*- */

/*
Bit manipulation functions
we can deal with individual bits with an unit (byte, word,...)
or with groups of bits (bitPart)
*/

namespace OneBit {
  typedef uint8_t Byte;

  //log base 2
  constexpr inline unsigned char _log2(size_t n) {return (n?_log2(n>>1)+1:0);};
  constexpr inline unsigned char log2(size_t n) {return n?_log2(n-1):0;};

  //bit operations within a bit field
  //operations get/set, on/off can cross bounds between units
  template<typename Unit>
  struct Bits {
    //bits per unit
    constexpr static inline Byte bpu() {return sizeof(Unit)<<3;}
    //address bits per unit
    constexpr static inline Byte abpu() {return log2(bpu());}
    //max representable value
    constexpr static inline Unit ones(Unit sz) {return (1<<sz)-1;}
    //mask
    constexpr static inline Unit mask(Byte bit=0,Byte sz=1) {return ones(sz)<<bit;}
    //unit index
    constexpr static inline Byte idx(Byte at) {return at>>abpu();}
    //bit address inside unit
    constexpr static inline Byte pos(Byte at) {return at&(ones(abpu()));}
    //get bit-field value
    constexpr static inline Unit get(Unit data[],Byte at,Byte sz) {
      // static_assert(sz<=bpu(),"bits field is limited to unit size");
      return (pos(at)+sz)>bpu()?//crossing bounds
        get(data,at,bpu()-pos(at))|get(data,at+(bpu()-pos(at)),sz-(bpu()-pos(at)))<<(bpu()-pos(at)):
        (data[idx(at)]>>pos(at))&(ones(sz));
    }
    //set bit-field value
    constexpr static inline void set(Unit data[],Byte at,Byte sz,Unit value) {
      Unit m=mask(pos(at),sz);
      if ((pos(at)+sz)>bpu()) {//crossing bounds
        set(data,at,bpu()-pos(at),value);
        set(data,at+(bpu()-pos(at)),sz-(bpu()-pos(at)),value>>(bpu()-pos(at)));
      } else data[idx(at)]=(data[idx(at)]&~m)|((value<<pos(at))&m);
    }
    //turn bit-field on (all ones)
    constexpr static inline void on(Unit data[],Byte at,Byte sz) {
      set(data,at,sz,ones(sz));
    }
    //turn bit-field off (all zeros)
    constexpr static inline void off(Unit data[],Byte at,Byte sz) {
      set(data,at,sz,0);
    }

  };

  // a sequence of bits on arbitrary data
  template<typename Unit,Byte at, Byte sz=1>
  struct BitPart {
    constexpr static inline Unit get(Unit data[]) {
      return Bits<Unit>::get(data,at,sz);
    }
    constexpr static inline void on(Unit data[]) {
      Bits<Unit>::on(data,at);
    }
    constexpr static inline void off(Unit data[]) {
      Bits<Unit>::off(data,at);
    }
    constexpr static inline void set(Unit data[],Unit value) {
      Bits<Unit>::set(data,at,sz,value);
    }
  };

  // just an array of data with functions to work with bits
  template<typename Unit,Unit data[]>
  struct BitData {

    constexpr static inline Unit get(Byte at,Byte sz) {
      return Bits<Unit>::get(data,at,sz);
    }

    template<Byte at, Byte sz=1>
    constexpr static inline Unit get() {return BitPart<Unit,at,sz>::get(data);}

    constexpr static inline void set(Byte at,Byte sz,Unit value) {
      return Bits<Unit>::set(data,at,sz,value);
    }

    template<Byte at, Byte sz=1>
    constexpr static inline void set(Unit value) {
      return BitPart<Unit,at,sz>::set(data,value);
    }
  };

  template<typename Unit, Unit data[],Byte at, Byte sz=1>
  struct BitField:public BitPart<Unit,at,sz> {
    constexpr static inline Unit get() {return BitPart<Unit,at,sz>::get(data);}
    constexpr static inline void on() {BitPart<Unit,at,sz>::on(data);}
    constexpr static inline void off() {BitPart<Unit,at,sz>::off(data);}
    constexpr static inline void set(Unit value) {BitPart<Unit,at,sz>::set(data,value);}
  };

};
