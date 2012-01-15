#include "encode.h"

namespace dfjson
{


    /*
        Encoding functions
    */

    /*
        Pointers
    */

    // A pointer to an unknown
    js::Value encode(jsmap & json, void* rval) {
        return js::Value();
    }

    /*
        Primitives - Integers
    */

    // must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double

    js::Value encode(jsmap & json, bool & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, int rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, long & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(jsmap & json, long long & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, short & rval){
        return js::Value((int)rval);
    }
    js::Value encode(jsmap & json, signed char & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned char & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned int & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(jsmap & json, unsigned long & rval){
        return js::Value((unsigned long long)rval);
    }
    js::Value encode(jsmap & json, unsigned long long & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned short & rval){
        return js::Value(rval);
    }

    /*
        STD Types
    */

    js::Value encode(jsmap & json, std::string & rval) {
        log(json, &rval, rval);
        return js::Value(rval);
    }
}