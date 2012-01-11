#ifndef DF_ENCODE
#define DF_ENCODE

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

#include <encode-df.h>
//#include <extract.h>

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;

/////
///// Templates
/////

// Deref everything

template<typename T> T & deref(T &v) { return v; }
template<typename T> T & deref(T *v) { return deref(*v); }

template<typename T> T * ref(T &v) { return &v; }
template<typename T> T * ref(T *v) { return v; }

/////
///// Encode stuff
/////

// general case
template <class T>
Value encode(T x) {
    return Value(_encode(x));
}
// A pointer to something
template <class T>
Value encode(T* x) {
    return encode(*x);
}
// An array of some type
template <class T, std::size_t N>
Value encode(T (&array)[N]){
    return Value("Not Implemented");
}
// A pointer to void
template <>
Value encode(void* x) {
    return Value();
}



// encode object

template <class T>
Object encode_object(T x) {
    return _encode(x);
}

template <class T>
Object encode_object(T* x) {
    return encode_object(*x);
}

//encode class with protected destructor

template <class T>
Object encode_class(T x) {
    return _encode(x);
}



// Vectors

template<typename T>
Value encode_vector(T val) {
    return encode(val);
}

template<typename T>
Value encode_vector(std::vector<T> vec) {
    Array out;
    for (int i=0; i < vec.size(); i++){
        out.push_back( encode_vector(vec[i]) );
    }
    return Value(out);
}


template<typename T>
Value _encode(std::vector<T> vec) {
    return encode_vector(vec);
}

// Arrays
template <class T, std::size_t N>
Value encode_array(T (&array)[N]){
    return Value("Not Implemented");
}


// Enums
template <class T, class B, class TB>
Value encode_enum(TB e){
    return Value("Not Implemented");
}

template <class T, class TB>
Value encode_enum(TB e){
    return Value("Not Implemented");
}

/////
///// Declarations
/////

// Strings
Value _encode(std::string rval);

// Ints of varius types
Value _encode(int8_t rval);
Value _encode(int16_t rval);
Value _encode(int32_t rval);
Value _encode(uint8_t rval);
Value _encode(uint16_t rval);
Value _encode(uint32_t rval);

Value _encode(float rval);

// Void
Value _encode(void* rval);

#endif
