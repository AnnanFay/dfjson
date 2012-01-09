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

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;

// Deref everything
template<typename T> T & dereference(T &v);
template<typename T> T & dereference(T *v);
/*
template <typename T> Value encode(T x);
template <typename T> Value encode(T* x);
*/
// Vectors
template <class VecType>
Value encode_vector(VecType vec);

// Arrays
template <class ArrType>
Value encode_array(ArrType arr, int size);

// Strings
Value _encode(std::string rval);

// Ints of varius types
Value _encode(int8_t rval);
Value _encode(int16_t rval);
Value _encode(int32_t rval);
Value _encode(uint8_t rval);
Value _encode(uint16_t rval);
Value _encode(uint32_t rval);

#endif