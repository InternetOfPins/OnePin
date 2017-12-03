/* -*- C++ -*- */
// #ifndef ONEFUNC_DEF
//   #define ONEFUNC_DEF

  // namespace OneLib {
    //function handler ------------------------------------------------------
    template<typename... types> struct Func;

    template<>
    struct Func<void> {
      template<void (*fn)()>
      struct With {
        static inline void caller() {(*fn)();}
      };
    };

    template<typename res>
    struct Func<res> {
      template<res (*fn)()>
      struct With {
        static inline res caller() {return (*fn)();}
      };
    };

    template<typename res,typename... types>
    struct Func<res,types...> {
      template<res (*fn)(types...),types... args>
      struct With {
        static inline res caller() {return (*fn)(args...);}
      };
    };

    //object method handler ------------------------------------------------
    template<typename... types> struct MFunc;

    template<typename O>
    struct MFunc<O> {
      template<O* o,void (O::*fn)()>
      struct With {
        static inline void caller() {(o->*fn)();}
      };
    };

    template<typename O,typename res>
    struct MFunc<O,res> {
      template<O* o,res (O::*fn)()>
      struct With {
        static inline res caller() {return (o->*fn)();}
      };
    };

    template<typename O,typename res,typename... types>
    struct MFunc<O,res,types...> {
      template<O* o,res (O::*fn)(types...),types... args>
      struct With {
        static inline res caller() {return (o->*fn)(args...);}
      };
    };
    //object method handler ------------------------------------------------
    template<typename O,O* o,void (O::*fn)()>
    struct OFunc {static inline void caller() {(o->*fn)();}};

  // }

// #endif
