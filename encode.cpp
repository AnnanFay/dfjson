#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>
#include <json_spirit_reader_template.h>
#include <json_spirit_writer_template.h>

#include <DFHack.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>
#include <DataDefs.h>

#include <df-headers.h>

#include <encode.h>
#include <encode-df.h>

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;

// Deref everything

template<typename T> T & dereference(T &v) { return v; }
template<typename T> T & dereference(T *v) { return dereference(*v); }

template <class T>
Value encode(T x) {
    return _encode(dereference(x));
}

template <class T>
Value encode(T* x) {
    return _encode(dereference(x));
}

// Vectors
template <class VecType>
Value encode_vector(VecType vec){
    Array out;
    for (int i=0; i < vec.size(); i++){
        out.push_back( encode(vec[i]) );
    }
    return Value(out);
}

// Arrays
template <class ArrType>
Value encode_array(ArrType arr, int size){
    return Value("Not Implemented");
}

// Strings
Value _encode(std::string rval) {
    return Value(rval);
}

// Ints of varius types
Value _encode(int8_t rval) {
    return Value(rval);
}
Value _encode(int16_t rval) {
    return Value(rval);
}
Value _encode(int32_t rval) {
    return Value(rval);
}
Value _encode(uint8_t rval) {
    return Value(rval);
}
Value _encode(uint16_t rval) {
    return Value(rval);
}
Value _encode(uint32_t rval) {
    return Value((long long)rval);
}
