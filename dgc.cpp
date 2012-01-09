// This is the first C/C++ program I've written. Apollogies for any stupid code.

#include <DFHack.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>

#include <encode.h>

#include <boost/lexical_cast.hpp>

#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace json_spirit;
using namespace boost;

#include <DataDefs.h>
#include <df/world.h>
#include <df/unit.h>

using namespace DFHack;
using namespace df::enums;

using df::global::world;
using df::unit;


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
    
    ofstream os( "Dwarves.json" );
    write_stream( encode(df::world), os, single_line_arrays | remove_trailing_zeros);
    os.close();

    return CR_OK;
}
