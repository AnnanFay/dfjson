// having guards seem to cause errors!!! WTF?
//#ifndef DF_ENCODE_DF_H
//#define DF_ENCODE_DF_H

#include <vector>
#include <string>
#include <typeinfo>

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
typedef std::map<std::string, js::Object> jsmap;


namespace dfjson
{
