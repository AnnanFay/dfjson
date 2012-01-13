#include <vector>
#include <string>

#include <json_spirit/json_spirit_reader_template.h>
#include <json_spirit/json_spirit_writer_template.h>

#include <DataDefs.h>

#include <df-headers.h>
#include <encode.h>


// Are these all needed?
#include <DFHack.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>

using namespace std;
using namespace json_spirit;
namespace js = json_spirit;
using namespace boost;
using namespace DFHack;


namespace dfjson
{

    /*
        Encoding functions
    */

    /*
        Pointers
    */

    // A pointer to an unknown
    js::Value encode(void* rval) {
        return js::Value();
    }

    /*
        Primitives - Integers
    */

    // must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double

    js::Value encode(bool & rval){
        return js::Value(rval);
    }
    js::Value encode(int rval){
        return js::Value(rval);
    }
    js::Value encode(long & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(long long & rval){
        return js::Value(rval);
    }
    js::Value encode(short & rval){
        return js::Value((int)rval);
    }
    js::Value encode(signed char & rval){
        return js::Value(rval);
    }
    js::Value encode(unsigned char & rval){
        return js::Value(rval);
    }
    js::Value encode(unsigned int & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(unsigned long & rval){
        return js::Value((unsigned long long)rval);
    }
    js::Value encode(unsigned long long & rval){
        return js::Value(rval);
    }
    js::Value encode(unsigned short & rval){
        return js::Value(rval);
    }

    /*
        STD Types
    */

    js::Value encode(std::string & rval) {
        return js::Value(rval);
    }