/* -*- C++ -*- */

/*
Bit manipulation functions
we can deal with individual bits with an unit (byte, word,...)
or with groups of bits (bitPart)
*/

namespace OneBit {
  typedef unsigned char Byte;

  //log base 2
  constexpr inline unsigned char _log2(size_t n) {return (n?_log2(n>>1)+1:0);};
  constexpr inline unsigned char log2(size_t n) {return n?_log2(n-1):0;};

  //bit operations within a bits unit (8|16|32|64 ... 255)
  template<typename Unit>
  struct Bits {
    constexpr static inline Unit ones(Unit sz) {return (1<<sz)-1;}
    constexpr static inline Unit mask(Byte bit=0,Byte sz=1) {
      return ones(sz)<<bit;
    }
    constexpr static inline Byte idx(Byte at) {
      return at>>log2(sizeof(Unit)<<3);
    }
    constexpr static inline Byte pos(Byte at) {
      return at&(ones(log2(sizeof(Unit)<<3)));
    }
    constexpr static inline Unit get(Unit data[],Byte at,Byte sz) {
      return (data[idx(at)]>>pos(at))&(ones(log2(sz<<3)));
    }
    constexpr static inline void set(Unit data[],Byte at,Byte sz,Unit value) {
      data[idx(at)]=
        (data[idx(at)]&~(mask(pos(at))))
        |((value&(ones(log2(sz<<3))))<<pos(at));
    }
    constexpr static inline void on(Unit data[],Byte at) {
      set(data,at,1,1);
    }
    constexpr static inline void off(Unit data[],Byte at) {
      set(data,at,1,0);
    }
  };

  // get set a sequence of bits (must not cross Unit boundary)
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

  //sequence of sz bits
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
};


//use case-----------------
/*typedef BitPart<uint8_t,20,4> Test;
uint8_t data[3]={0,0,0};
typedef BitData<uint8_t,data> BitData24;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial<<"The thing"<<endl;
  // Test::set(data,0b1001);
  BitData24::set<20,4>(0b1011);
  Serial<<bin(BitData24::get<20,4>())<<endl;
}
*/
