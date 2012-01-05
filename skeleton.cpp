// This is a generic plugin that does nothing useful apart from acting as an example... of a plugin that does nothing :D

// some headers required for a plugin. Nothing special, just the basics.
#include <dfhack/Core.h>
#include <dfhack/Console.h>
#include <dfhack/Export.h>
#include <dfhack/PluginManager.h>
using namespace DFHack;

// our own, empty header.
#include "skeleton.h"


// Here go all the command declarations...
// mostly to allow having the mandatory stuff on top of the file and commands on the bottom
DFhackCExport command_result skeleton (Core * c, std::vector <std::string> & parameters);

// A plugins must be able to return its name. This must correspond to the filename - skeleton.plug.so or skeleton.plug.dll
DFhackCExport const char * plugin_name ( void )
{
    return "skeleton";
}

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init ( Core * c, std::vector <PluginCommand> &commands)
{
    // Fill the command list with your commands.
    commands.clear();
    commands.push_back(PluginCommand("skeleton",
                                     "Do nothing, look pretty.",
                                     skeleton /*,
                                     true or false - true means that the command can't be used from non-interactive user interface'*/));
    return CR_OK;
}

// This is called right before the plugin library is removed from memory.
DFhackCExport command_result plugin_shutdown ( Core * c )
{
    // You *MUST* kill all threads you created before this returns.
    // If everythin fails, just return CR_FAILURE. Your plugin will be
    // in a zombie state, but things won't crash.
    return CR_OK;
}

// Whatever you put here will be done in each game step. Don't abuse it.
// It's optional, so you can just comment it out like this if you don't need it.
/*
DFhackCExport command_result plugin_onupdate ( Core * c )
{
    // whetever. You don't need to suspend DF execution here.
    return CR_OK;
}
*/

// A command! It sits around and looks pretty. And it's nice and friendly.
DFhackCExport command_result skeleton (Core * c, std::vector <std::string> & parameters)
{
    // It's nice to provide a 'help' option for your command.
    // It's also nice to print the same help if you get invalid options from the user instead of just acting strange
    for(int i = 0; i < parameters.size();i++)
    {
        if(parameters[i] == "help" || parameters[i] == "?")
        {
            // Core has a handle to the console. The console is thread-safe.
            // Only one thing can read from it at a time though...
            c->con.print("This command does nothing!\n");
            return CR_OK;
        }
    }
    // Commands are called from threads other than the DF one.
    // Suspend this thread until DF has time for us.
    c->Suspend();
    // Actually do something here. Yay.
    c->con.print("Hello! I do nothing, remember?\n");
    // Give control back to DF.
    c->Resume();
    return CR_OK;
}
