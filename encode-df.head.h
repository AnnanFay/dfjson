#ifndef DF_ENCODE_DF_H
#define DF_ENCODE_DF_H
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

#include <encode-df.h>
#include <encode.h>

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;

