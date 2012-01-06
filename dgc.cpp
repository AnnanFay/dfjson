// This is the first C/C++ program I've written. Apollogies for any stupid code.

#include <dfhack/Core.h>
#include <dfhack/Console.h>
#include <dfhack/Export.h>
#include <dfhack/PluginManager.h>
#include <vector>
#include <string>
#include <dfhack/modules/Units.h>
#include <dfhack/modules/Translation.h>
#include <dfhack/df/static.inc>


#include <boost/lexical_cast.hpp>

//#include <json_spirit.h>
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace DFHack;
using namespace json_spirit;
using namespace boost;

#include <DFHack.h>

DFhackCExport command_result dgc (Core * c, vector <string> & parameters);

DFhackCExport const char * plugin_name ( void )
{
    return "dgc";
}

DFhackCExport command_result plugin_init ( Core * c, std::vector <PluginCommand> &commands)
{
    commands.clear();
    commands.push_back(PluginCommand("dgc",
               "DGC DESCRIPTION",
                dgc));
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( Core * c )
{
    return CR_OK;
}


void   printUnits  (Core * c);
Array  getDwarves  (Core * c);
Object getDwarf    (df_unit * dwarf, DFHack::Translation * Tran, DFHack::VersionInfo * Mem);

DFhackCExport command_result dgc (Core * c, vector <string> & parameters)
{
    // Suspend DF
    c->Suspend();
    
    Array dwarves = getDwarves(c);
    
    ofstream os( "Dwarves-0.json" );
    write_stream( Value( dwarves ), os, single_line_arrays | remove_trailing_zeros);
    os.close();
    
    //c->con << write( dwarves );
    
    // We've done everything so resume
    c->Resume();
    return CR_OK;
}


Array getDwarves(Core *c) {

    DFHack::Units       * Units      = c->getUnits();
    DFHack::Translation * Tran       = c->getTranslation();
    //DFHack::Materials   * Materials  = c->getMaterials();
    DFHack::VersionInfo * Mem        = c->vinfo;

    // Why? 
    Tran->Start();
    
    static vector <df_unit*> * unitVec = Units->creatures;
    uint32_t dwarven                   = Units->GetDwarfRaceIndex();
    uint32_t civ                       = Units->GetDwarfCivId();

    df_unit * unit;
    Array dwarves_vec;
    Object dwarf_obj;
    for (unsigned i=0; i < unitVec->size(); ++i)
    {
        unit = unitVec->at(i);
        if (unit->race == dwarven && unit->civ == civ) // && cre->job_counter > 0
        {
            dwarf_obj = getDwarf(unit, Tran, Mem);
            dwarves_vec.push_back( dwarf_obj );
        }
    }

    Units->Finish();
    //Materials->Finish();
    Tran->Finish();
    
    return dwarves_vec;
}

void add(Object dwarf, string name, Value val){
    dwarf.push_back( Pair(name, val) );
}

Value to_value(df_attrib df_attr, string name = ""){
    Object attr;

    attr.push_back( Pair("name",           Value(name)));
    attr.push_back( Pair("rating",         Value((long long)df_attr.unk_0)));
    attr.push_back( Pair("unk_4",          Value((long long)df_attr.unk_4)));
    attr.push_back( Pair("unk_8",          Value((long long)df_attr.unk_8)));
    attr.push_back( Pair("unk_c",          Value((long long)df_attr.unk_c)));
    attr.push_back( Pair("unk_10",         Value((long long)df_attr.unk_10)));
    attr.push_back( Pair("unk_14",         Value((long long)df_attr.unk_14)));
    attr.push_back( Pair("unk_18",         Value((long long)df_attr.unk_18)));
 
    return Value(attr);
}

Value to_value(df_skill * df_sk, DFHack::VersionInfo * Mem){
    Object skill;
 
    skill.push_back( Pair("name",       Value(Mem->getSkill(df_sk->id))));
    skill.push_back( Pair("id",         Value((long long)df_sk->id)));
    skill.push_back( Pair("rating",     Value((long long)df_sk->rating)));
    skill.push_back( Pair("experience", Value((long long)df_sk->experience)));
    skill.push_back( Pair("unk_c",      Value((long long)df_sk->unk_c)));
    skill.push_back( Pair("unk_rusty",  Value((long long)df_sk->rusty)));
    skill.push_back( Pair("unk_14",     Value((long long)df_sk->unk_14)));
    skill.push_back( Pair("unk_18",     Value((long long)df_sk->unk_18)));
    skill.push_back( Pair("unk_1c",     Value((long long)df_sk->unk_1c)));
    
    return Value(skill);
}


Value getTraits(df_unit * dwarf, DFHack::VersionInfo * Mem) {
    uint16_t * dwarf_traits = dwarf->current_soul->traits;
    Object traits;
    
    for (int i=0; i < NUM_CREATURE_TRAITS; i++){
        traits.push_back(Pair(Mem->getTraitName((uint32_t)i), dwarf_traits[i]));
    }
           
    return Value(traits);
}

Value getLabors(df_unit * dwarf, DFHack::VersionInfo * Mem) {
    uint8_t * dwarf_labors = dwarf->labors;
    Object labors;
    
    for (int i=0; i < NUM_CREATURE_LABORS; i++){
        try {
            labors.push_back(Pair(Mem->getLabor((uint32_t)i), dwarf_labors[i]));
        } catch (...){
            labors.push_back(Pair("unknown-" + lexical_cast<string>(i), dwarf_labors[i]));
        }
    }
           
    return Value(labors);
}

Object getDwarf(df_unit * dwarf, DFHack::Translation * Tran, DFHack::VersionInfo * Mem){
    Object dwarf_obj;

    df_soul   * soul     = dwarf->current_soul;
    df_attrib * physical = dwarf->physical;

    df_attrib * mental   = soul->mental;

    // Name
    Object name;
    string given_name = dwarf->name.first_name;
    given_name[0] = toupper(given_name[0]);

    name.push_back( Pair("given-name",   Value(given_name)) );
    name.push_back( Pair("nickname",    Value(dwarf->name.nick_name)) );
    name.push_back( Pair("surname",     Value(Tran->TranslateName(&dwarf->name, false))) );

    dwarf_obj.push_back( Pair("name", Value(name)) );

    // Profession
    dwarf_obj.push_back( Pair("profession",         Value(Mem->getProfession(dwarf->profession))) );
    dwarf_obj.push_back( Pair("customProfession",   Value(dwarf->custom_profession)) );

    // Race
    dwarf_obj.push_back( Pair("race",               Value((long long)dwarf->race)) );

    // Position
    Object pos;
     
    pos.push_back( Pair("x", Value(dwarf->x)) );
    pos.push_back( Pair("y", Value(dwarf->y)) );
    pos.push_back( Pair("z", Value(dwarf->z)) );

    dwarf_obj.push_back( Pair("position", Value(pos)) );

    // Destination
    Object dest;
     
    dest.push_back( Pair("x", Value(dwarf->dest_x)) );
    dest.push_back( Pair("y", Value(dwarf->dest_y)) );
    dest.push_back( Pair("z", Value(dwarf->dest_z)) );

    dwarf_obj.push_back( Pair("destination", Value(dest)) );

    // Caste
    dwarf_obj.push_back( Pair("caste",          Value((long long)dwarf->caste)) );

    // Sex
    dwarf_obj.push_back( Pair("sex",            Value((long long)dwarf->sex)) );

    // ID
    dwarf_obj.push_back( Pair("id",             Value((long long)dwarf->id)) );

    // Civ
    dwarf_obj.push_back( Pair("civilization",   Value((long long)dwarf->civ)) );

    //Mood
    try { 
    dwarf_obj.push_back( Pair("mood",           Value(Mem->getMood(dwarf->mood))));
    } catch (...){}
   
    //Birth Stuff
    dwarf_obj.push_back( Pair("pregnancy-timer",Value((long long)dwarf->pregnancy_timer)));
    dwarf_obj.push_back( Pair("birth-year",     Value((long long)dwarf->birth_year)));
    dwarf_obj.push_back( Pair("birth-time",     Value((long long)dwarf->birth_time)));
    
    
    //df_unit * unk_234; // suspiciously close to the pregnancy/birth stuff. Mother?

    //std::vector<df_item *> inventory;   // 288 - vector of item pointers
    //std::vector<int32_t> owned_items;  // 298 - vector of item IDs

    //df_job * current_job;   // 2f4

    // Stats
    Object stats;
    Array skills, likes, physical_attrs, mental_attrs;

    // Stats - Physical
    // Convert array to vector
    df_attrib * p_phy = dwarf->physical;

    string physical_attr_names[NUM_CREATURE_PHYSICAL_ATTRIBUTES] = {
        "strength",
        "agility",
        "toughness",
        "endurance",
        "recuperation",
        "disease-resistance"
    };





    for (int i=0; i < NUM_CREATURE_PHYSICAL_ATTRIBUTES; i++){
        physical_attrs.push_back( to_value(p_phy[i], physical_attr_names[i]) );
    } 

    stats.push_back( Pair("physical",   Value(physical_attrs)) );

    // Stats - Mental

     // Convert array to vector
    df_attrib * p_men = dwarf->physical;
    vector<df_attrib> d_men;
    d_men.assign(p_men, p_men + NUM_CREATURE_MENTAL_ATTRIBUTES);
    
    for (int i=0; i < d_men.size(); i++){
        mental_attrs.push_back( to_value(d_men[i]) );
    } 
    
    stats.push_back( Pair("mental",     Value(mental_attrs)) );
    
    // Stats - Skills
    
    vector<df_skill*> dskills = dwarf->current_soul->skills;
    
    for (int i=0; i < dskills.size(); i++){
        skills.push_back( to_value(dskills[i], Mem) );
    }    
    
    stats.push_back( Pair("skills",     Value(skills)) );
    
    // Stats - Likes
    vector<df_like*> dlikes   = dwarf->current_soul->likes;
    
    for (int i=0; i < dlikes.size(); i++){
        Object like;
       
        like.push_back( Pair( "type",              Value((long long)dlikes[i]->type)));
        like.push_back( Pair( "itemClass",         Value((long long)dlikes[i]->itemClass)));
        like.push_back( Pair( "itemIndex",         Value((long long)dlikes[i]->itemIndex)));
        like.push_back( Pair( "material_type",     Value((long long)dlikes[i]->material_type)));
        like.push_back( Pair( "material_index",    Value((long long)dlikes[i]->material_index)));
        like.push_back( Pair( "active",            Value(dlikes[i]->active)));
        like.push_back( Pair( "mystery",           Value((float)dlikes[i]->mystery)));
        
        likes.push_back( Value(like) );
    }    
    
    stats.push_back( Pair("likes",     Value(likes)) );
    
    // Stats - Traits
    const Value traits = getTraits(dwarf, Mem);
    stats.push_back( Pair("traits",     traits) );

    // Stats - Add dwarf object
    dwarf_obj.push_back( Pair("stats",  Value(stats)) );
 
    //df_name artifact_name; // 5e0
    
    // Labors
    const Value labors = getLabors(dwarf, Mem);
    dwarf_obj.push_back( Pair("labors",     labors) );
    
    //std::vector<df_thought *> thoughts;
    
    dwarf_obj.push_back( Pair("happiness",          Value((long long)dwarf->happiness)));
    dwarf_obj.push_back( Pair("hist-figure-id",     Value((long long)dwarf->hist_figure_id)));
    dwarf_obj.push_back( Pair("able-stand",         Value((long long)dwarf->able_stand)));
    dwarf_obj.push_back( Pair("able-stand-impair",  Value((long long)dwarf->able_stand_impair)));
    dwarf_obj.push_back( Pair("able-grasp",         Value((long long)dwarf->able_grasp)));
    dwarf_obj.push_back( Pair("able-grasp-impair",  Value((long long)dwarf->able_grasp_impair)));

    return dwarf_obj;
}

