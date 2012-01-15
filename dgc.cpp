// This is the first C/C++ program I've written. Apollogies for any stupid code.

#include "dgc.h"

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

template<class T>
void write_json(T obj, std::string filename){
    Value val = encode_to_globals(obj);

    ofstream os( filename );
    write_stream( val, os, single_line_arrays | remove_trailing_zeros);
    os.close();
}

DFhackCExport command_result dgc (Core * c, vector <string> & parameters)
{
    // Suspend DF
    CoreSuspender suspend(c);

    write_json(world->units.all, "export/Dwarves.json");

    return CR_OK;
}
