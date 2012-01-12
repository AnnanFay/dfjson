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

Value _encode(float rval) {
    return Value(rval);
}

Value _encode(void* rval) {
    return Value();
}