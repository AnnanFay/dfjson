
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>

#include <DataDefs.h>
#include <df/world.h>
#include <df/unit.h>

#include <boost/lexical_cast.hpp>

#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"

#include "encode-df.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace json_spirit;
using namespace boost;
using namespace DFHack;
using namespace df::enums;

using df::global::world;
using df::unit;
