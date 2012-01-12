#ifndef DF_ENCODE
#define DF_ENCODE

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>
#include <json_spirit/json_spirit_reader_template.h>
#include <json_spirit/json_spirit_writer_template.h>

#include <DFHack.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>
#include <DataDefs.h>

#include <df-headers.h>

#include <encode-df.h>

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;

/*
    Encoding functions
*/

/*
    General Case
*/
template <class T>
json_spirit::Value encode(T rval) {
    // Don't assume that it can be encoded by something else, just pass back nothing
    return json_spirit::Value();
}

/*
    Pointers
*/
template <class T>
json_spirit::Value encode(T* rval) {
    // deref and encode
    return encode(*rval);
}

// A pointer to an unknown
json_spirit::Value encode(void* rval) {
    return json_spirit::Value();
}

/*
    Primitives - Integers
*/

// must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double

json_spirit::Value encode(bool rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(int rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(long rval){
    return json_spirit::Value((long long)rval);
}
json_spirit::Value encode(long long rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(short rval){
    return json_spirit::Value((int)rval);
}
json_spirit::Value encode(signed char rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(unsigned char rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(unsigned long rval){
    return json_spirit::Value((unsigned long long)rval);
}
json_spirit::Value encode(unsigned long long rval){
    return json_spirit::Value(rval);
}
json_spirit::Value encode(unsigned short rval){
    return json_spirit::Value(rval);
}

/*
    Primitives - Arrays
*/

template <class T, std::size_t N>
json_spirit::Value encode(T (&rval)[N]){
    Array out;
    for (int i=0; i < N; i++){
        out.push_back( encode(rval[i]) );
    }
    return json_spirit::Value(out);
}

/*
    STD Types
*/

json_spirit::Value encode(std::string rval) {
    return json_spirit::Value(rval);
}

template<class T>
json_spirit::Value encode(std::vector<T> rval) {
    Array out;
    for (int i=0; i < rval.size(); i++){
        out.push_back( encode(rval[i]) );
    }
    return json_spirit::Value(out);
}


/*
    DF Types
*/
template<class EnumType, class IntType>
json_spirit::Value encode(df::enum_field<EnumType,IntType> rval) {
    return json_spirit::Value();
}

/*
    template<class EnumType, class IntType = int32_t>
    struct enum_field {
        IntType value;

        enum_field() {}
        enum_field(EnumType ev) : value(IntType(ev)) {}
        template<class T>
        enum_field(enum_field<EnumType,T> ev) : value(IntType(ev.value)) {}

        operator EnumType () { return EnumType(value); }
        enum_field<EnumType,IntType> &operator=(EnumType ev) {
            value = IntType(ev); return *this;
        }
    };
*/

/*
template<>
json_spirit::Value encode(df::history_event* rval){
    Object val;
    val.push_back(Pair("year",                                encode(rval->year)));
    val.push_back(Pair("seconds",                             encode(rval->seconds)));
    val.push_back(Pair("flags",                               encode(rval->flags)));
    val.push_back(Pair("id",                                  encode(rval->id)));
    return json_spirit::Value(val);
}
*/

#endif
