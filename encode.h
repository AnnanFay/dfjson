#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>
#include <cxxabi.h> 

// REMOVE
#include <iostream>
#include <fstream>
// ^THOSE

#include <json_spirit/json_spirit_writer_template.h>

#include <DataDefs.h>
//#include <DFHack.h>

#include "df-headers.h"
#include "encode-df.h"

using namespace std;
using namespace json_spirit;
namespace js = json_spirit;

using df::global::world;

typedef std::map<std::string, js::Object> jsmap;

namespace dfjson
{

    // DECLARATIONS

    template <class T>
    void log(jsmap & json, T * rval);
    template <class T>
    void log(jsmap & json, T & rval);
    template <class T, class M>
    void log(jsmap & json, T & rval, M msg);
    template <class T, class M>
    void log(jsmap & json, T * rval, M msg);
    template <class M>
    void log(M msg);
    // Templates
    template <class T>                          js::Value encode_to_globals(T * rval);
    template <class T>                          js::Value encode_to_globals(T & rval);

    template <class T>                          js::Value encode(jsmap &, T & rval);
    template <class T>                          js::Value encode(jsmap &, T * rval);
    template <class T, std::size_t N>           js::Value encode(jsmap &, T (&rval)[N]);

    template <class T>                          js::Value encode_array(jsmap &, T *, std::size_t);

    template <class T>                          js::Value encode(jsmap &, std::vector<T> & rval);
    template <class EnumType, class IntType>    js::Value encode(jsmap &, df::enum_field<EnumType,IntType> & rval);

    template <class T>                          std::string get_p           (T * o);
    template <class T>                          std::string global_exists   (jsmap &, T *);
    template <class T>                          std::string global_add      (jsmap &, T *, js::Object&);

    // Functions

    // ints must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double
    js::Value encode(jsmap &, bool & rval);
    js::Value encode(jsmap &, int rval);
    js::Value encode(jsmap &, long & rval);
    js::Value encode(jsmap &, long long & rval);
    js::Value encode(jsmap &, short & rval);
    js::Value encode(jsmap &, signed char & rval);
    js::Value encode(jsmap &, unsigned char & rval);
    js::Value encode(jsmap &, unsigned int & rval);
    js::Value encode(jsmap &, unsigned long & rval);
    js::Value encode(jsmap &, unsigned long long & rval);
    js::Value encode(jsmap &, unsigned short & rval);
    js::Value encode(jsmap &, std::string & rval);
    js::Value encode(jsmap &, void* rval);


    js::Value encode(jsmap &, std::string & rval);
    js::Value encode(jsmap &, void* rval);

    js::Value translate(jsmap & json, df::language_name * name);
    js::Value get_race (jsmap & json, df::unit          * unit);

    js::Value encode(jsmap & json, df::creature_raw* rval);
    js::Value encode(jsmap & json, df::creature_raw& rval);


    // TEMPLATE DEFINITIONS
    /*
        Encoding functions
    */

    /*
        General Case
    */
    
    template <class T>
    js::Value encode(jsmap &, T & rval) {
        // we don't know how to render it so pass back it's type for debugging purposes
        return js::Value(abi::__cxa_demangle(typeid(T).name(), 0, 0, 0));
    }

    /*
        Pointers
    */
    template <class T>
    js::Value encode(jsmap & json, T* rval) {
        log(json, rval);

        // There are serious problems with infinite recursive structures if we encode pointers
        // There are three solutions
        // 1. Limit recursion depth by using a global counter (bad) or passing an argument down the chain (better)
        // 2. Encoding breadth first and detecting if the current object has already been rendered. (may have problems)
        // 3. If something is a pointer don't render it, just render an artificial pointer/id to where it is (best solution?)
        // 4. The clojure code could detect fields that are the same class as the one of the ancestors and not ask for them to be rendered

        // NULL Pointer
        if (!rval){
            return js::Value();
        }

        // deref and encode
        return encode(json, *rval);
    }

    /*
        Primitives - Arrays
    */

    template <class T, std::size_t N>
    js::Value encode(jsmap & json, T (&rval)[N]){
        log("encoding array");
        log(json, rval, N);
        js::Array out;
        for (int i=0; i < N; i++){
            out.push_back( encode(json, rval[i]) );
        }
        return js::Value(out);
    }

    // See if this works
    template <class T>
    js::Value encode_array(jsmap & json, T * rval, std::size_t N){
        log("encoding array");
        log(json, rval, N);
        js::Array out;
        for (int i=0; i < N; i++){
            out.push_back( encode(json, rval[i]) );
        }
        return js::Value(out);
    }

    /*
        STD Types
    */

    template<class T>
    js::Value encode(jsmap & json, std::vector<T> &rval) {
        log(json, rval);
        js::Array out;
        for (int i=0; i < rval.size(); i++){
            out.push_back( encode(json, rval[i]) );
        }
        return js::Value(out);
    }


    /*
        DF Types
    */
    template <class EnumType, class IntType>
    js::Value encode(jsmap & json, df::enum_field<EnumType,IntType> & rval) {
        log(json, rval);
        //EnumType e = rval();
        //IntType v = rval.value;
        return js::Value(get_key(rval));
        //return js::Value(ENUM_KEY_STR(e, v));
    }

    /*
        Others
    */
    template<class T>
    js::Value encode_to_globals(T & rval) {
        return encode_to_globals(&rval);
    }
    template<class T>
    js::Value encode_to_globals(T * rval) {
        
        jsmap json;
        
        // encode has side effects!
        // it adds globals to json and returns a representation of rval!
        // NOT AN JS:ARRAY
        // BROKEN
        //json["root"] = encode(json, rval);
        js::Value root = encode(json, rval);

        // Convert our map to a value
        js::Object obj;
        obj.push_back(js::Pair( "root", root ) );

        for (jsmap::reverse_iterator iter = json.rbegin(); iter != json.rend(); ++iter) {
            obj.push_back(js::Pair( iter->first, iter->second ) );
        }
        
        return js::Value(obj);
    }

    template <class T>
    std::string get_p(T * o){
        const void * address = static_cast<const void*>(o);
        std::stringstream ss;
        ss << address;  
        return "p" + ss.str(); 
    }

    template <class T>
    std::string global_exists(jsmap & json, T * rval){
        std::string p = get_p(rval);
        if (json.find(p) == json.end()) {
            json[p];
            return "";
        } else {
            return p;
        }
    }

    
    template <class T>
    std::string global_add(jsmap & json, T * rval, js::Object & val){
        std::string p = get_p(rval);
        json[p] = val;
        return p;
    }

    template <class T>
    void log(jsmap & json, T * rval){
        log(json, rval, 0);
    }

    template <class T>
    void log(jsmap & json, T & rval){
        log(json, &rval, 0);
    }

    template <class T, class M>
    void log(jsmap & json, T & rval, M msg){
        log(json, &rval, msg);
    }

    template <class T, class M>
    void log(jsmap & json, T * rval, M msg){
        ofstream myfile;
        myfile.open ("encode.log", ios::out | ios::app);

        size_t len;
        int s;
        char* p = abi::__cxa_demangle(typeid(T).name(), 0, &len, &s);
        myfile << "TYPE: " << p << std::endl; 

        myfile << "    Pointer: " << get_p(rval) << endl;
        myfile << "    Message: " << msg << endl;
        myfile.close();
    }

    template <class M>
    void log(M msg){
        ofstream myfile;
        myfile.open ("encode.log", ios::out | ios::app);
        myfile << msg << endl;
        myfile.close();
    }

}