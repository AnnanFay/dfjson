#ifndef DF_ENCODE_DF
#define DF_ENCODE_DF
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

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;