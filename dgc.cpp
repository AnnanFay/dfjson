// This is the first C/C++ program I've written. Apollogies for any stupid code.

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
using namespace dfjson;

using df::global::world;

DFhackCExport command_result dgc (Core * c, vector <string> & parameters);

DFhackCExport const char * plugin_name ( void ) {
    return "dgc";
}

DFhackCExport command_result plugin_init ( Core * c, std::vector <PluginCommand> &commands) {
    commands.clear();
    commands.push_back(PluginCommand("dgc",
               "DGC DESCRIPTION",
                dgc));
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( Core * c ) {
    return CR_OK;
}


DFhackCExport command_result dgc (Core * c, vector <string> & parameters)
{
    // Suspend DF
    CoreSuspender suspend(c);

    //Value val = encode(world->units);
    Value val = encode(world->units.all);

    ofstream os( "Dwarves.json" );
    write_stream( val, os, single_line_arrays | remove_trailing_zeros);
    os.close();

    return CR_OK;
}
