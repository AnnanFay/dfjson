// This is the first C/C++ program I've written. Apollogies for any stupid code.

#include <dfhack/Core.h>
#include <dfhack/Console.h>
#include <dfhack/Export.h>
#include <dfhack/PluginManager.h>
#include <vector>
#include <string>
#include <dfhack/modules/Units.h>
#include <dfhack/modules/Translation.h>

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
Object getDwarf    (df_unit * dwarf, DFHack::Translation * Tran, DFHack::Materials * Materials);

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
    DFHack::Materials   * Materials  = c->getMaterials();
    
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
            dwarf_obj = getDwarf(unit, Tran, Materials);
            dwarves_vec.push_back( dwarf_obj );
        }
    }

    Units->Finish();
    Materials->Finish();
    Tran->Finish();
    
    return dwarves_vec;
}

void add(Object dwarf, string name, Value val){
    dwarf.push_back( Pair(name, val) );
}

Value to_value(df_attrib df_attr){
    Object attr;

    attr.push_back( Pair("unk_0",          Value((long long)df_attr.unk_0)));
    attr.push_back( Pair("unk_4",          Value((long long)df_attr.unk_4)));
    attr.push_back( Pair("unk_8",          Value((long long)df_attr.unk_8)));
    attr.push_back( Pair("unk_c",          Value((long long)df_attr.unk_c)));
    attr.push_back( Pair("unk_10",         Value((long long)df_attr.unk_10)));
    attr.push_back( Pair("unk_14",         Value((long long)df_attr.unk_14)));
    attr.push_back( Pair("unk_18",         Value((long long)df_attr.unk_18)));
 
    return Value(attr);
}


Value getTraits(df_unit * dwarf) {
    uint16_t * dwarf_traits = dwarf->current_soul->traits;
    Object traits;
    
    std::string trait_names[] = {
        "ANXIETY",
        "ANGER",
        "DEPRESSION",
        "SELF_CONSCIOUSNESS",
        "IMMODERATION",
        "VULNERABILITY",
        "FRIENDLINESS",
        "GREGARIOUSNESS",
        "ASSERTIVENESS",
        "ACTIVITY_LEVEL",
        "EXCITEMENT_SEEKING",
        "CHEERFULNESS",
        "IMAGINATION",
        "ARTISTIC_INTEREST",
        "EMOTIONALITY",
        "ADVENTUROUSNESS",
        "INTELLECTUAL_CURIOSITY",
        "LIBERALISM",
        "TRUST",
        "STRAIGHTFORWARDNESS",
        "ALTRUISM",
        "COOPERATION",
        "MODESTY",
        "SYMPATHY",
        "SELF_EFFICACY",
        "ORDERLINESS",
        "DUTIFULNESS",
        "ACHIEVEMENT_STRIVING",
        "SELF_DISCIPLINE",
        "CAUTIOUSNESS "
    };
    
    /*
    uint16_t * p_traits = dwarf->current_soul->traits;
    vector<uint16_t> d_traits;
    d_traits.assign(p_traits, p_traits + NUM_CREATURE_TRAITS);
    
    const Value traits( d_traits.begin(), d_traits.end() );
    */
    
    for (int i=0; i < NUM_CREATURE_TRAITS; i++){
        traits.push_back(Pair(trait_names[i], dwarf_traits[i]));
    }
           
    return traits;
}

Object getDwarf(df_unit * dwarf, DFHack::Translation * Tran, DFHack::Materials * Materials){
    Object dwarf_obj;

    df_soul   * soul     = dwarf->current_soul;
    uint8_t   * labors   = dwarf->labors;
    df_attrib * physical = dwarf->physical;

    df_attrib * mental   = soul->mental;

    // Name
    Object name;
     
    name.push_back( Pair("First",     Value(dwarf->name.first_name)) );
    name.push_back( Pair("NickFirst", Value(dwarf->name.nick_name)) );
    name.push_back( Pair("TranName",  Value(Tran->TranslateName(&dwarf->name, false))) );

    dwarf_obj.push_back( Pair("Name", Value(name)) );

    // Profession
    dwarf_obj.push_back( Pair("Profession", Value(dwarf->profession)) );
    dwarf_obj.push_back( Pair("CustomProfession", Value(dwarf->custom_profession)) );

    // Race
    dwarf_obj.push_back( Pair("Race", Value((long long)dwarf->race)) );

    // Position
    Object pos;
     
    pos.push_back( Pair("x", Value(dwarf->x)) );
    pos.push_back( Pair("y", Value(dwarf->y)) );
    pos.push_back( Pair("z", Value(dwarf->z)) );

    dwarf_obj.push_back( Pair("Position", Value(pos)) );

    // Destination
    Object dest;
     
    dest.push_back( Pair("x", Value(dwarf->dest_x)) );
    dest.push_back( Pair("y", Value(dwarf->dest_y)) );
    dest.push_back( Pair("z", Value(dwarf->dest_z)) );

    dwarf_obj.push_back( Pair("Destination", Value(dest)) );

    // Caste
    dwarf_obj.push_back( Pair("Caste",          Value((long long)dwarf->caste)) );

    // Sex
    dwarf_obj.push_back( Pair("Sex",            Value((long long)dwarf->sex)) );

    // ID
    dwarf_obj.push_back( Pair("ID",             Value((long long)dwarf->id)) );

    // Civ
    dwarf_obj.push_back( Pair("Civilization",   Value((long long)dwarf->civ)) );

    //Mood
    dwarf_obj.push_back( Pair("mood",           Value((long long)dwarf->mood)));
    
    dwarf_obj.push_back( Pair("pregnancy_timer",Value((long long)dwarf->pregnancy_timer)));
    dwarf_obj.push_back( Pair("birth_year",     Value((long long)dwarf->birth_year)));
    dwarf_obj.push_back( Pair("birth_time",     Value((long long)dwarf->birth_time)));
    
    
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
    vector<df_attrib> d_phy;
    d_phy.assign(p_phy, p_phy + NUM_CREATURE_PHYSICAL_ATTRIBUTES);
    
    for (int i=0; i < d_phy.size(); i++){
        physical_attrs.push_back( to_value(d_phy[i]) );
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
        Object skill;

        skill.push_back( Pair("id",         Value((long long)dskills[i]->id)));
        skill.push_back( Pair("rating",     Value((long long)dskills[i]->rating)));
        skill.push_back( Pair("experience", Value((long long)dskills[i]->experience)));
        skill.push_back( Pair("unk_c",      Value((long long)dskills[i]->unk_c)));
        skill.push_back( Pair("unk_rusty",  Value((long long)dskills[i]->rusty)));
        skill.push_back( Pair("unk_14",     Value((long long)dskills[i]->unk_14)));
        skill.push_back( Pair("unk_18",     Value((long long)dskills[i]->unk_18)));
        skill.push_back( Pair("unk_1c",     Value((long long)dskills[i]->unk_1c)));
        
        skills.push_back( Value(skill) );
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

    const Value traits = getTraits(dwarf);
    stats.push_back( Pair("traits",     traits) );

    // Stats - Add dwarf object
    dwarf_obj.push_back( Pair("stats",  Value(stats)) );
 

    //df_name artifact_name; // 5e0
    
    // Labors
    uint8_t * p_labors = dwarf->labors;
    vector<uint8_t> d_labors;
    d_labors.assign(p_labors, p_labors + NUM_CREATURE_LABORS);
     
    const Value v_labors( d_labors.begin(), d_labors.end() );

    dwarf_obj.push_back( Pair("labors",     v_labors) );

    //std::vector<df_thought *> thoughts;
    
    dwarf_obj.push_back( Pair("happiness",          Value((long long)dwarf->happiness)));
    dwarf_obj.push_back( Pair("hist_figure_id",     Value((long long)dwarf->hist_figure_id)));
    dwarf_obj.push_back( Pair("able_stand",         Value((long long)dwarf->able_stand)));
    dwarf_obj.push_back( Pair("able_stand_impair",  Value((long long)dwarf->able_stand_impair)));
    dwarf_obj.push_back( Pair("able_grasp",         Value((long long)dwarf->able_grasp)));
    dwarf_obj.push_back( Pair("able_grasp_impair",  Value((long long)dwarf->able_grasp_impair)));

    return dwarf_obj;
}
void printUnits(Core *c)
{
    DFHack::Units       *Units     = c->getUnits();
    DFHack::Translation *Tran      = c->getTranslation();
    DFHack::Materials   *materials = c->getMaterials();
    
    Tran->Start();
    
    static vector <df_unit*> *unitVec = Units->creatures;
    uint32_t race                     = Units->GetDwarfRaceIndex();
    uint32_t civ                      = Units->GetDwarfCivId();

    df_unit *cre;
    for (unsigned i=0; i < unitVec->size(); ++i)
    {
        cre = unitVec->at(i);
        if (cre->race == race && cre->civ == civ && cre->job_counter > 0)
        {
            df_soul * soul = cre->current_soul;
            uint8_t * labors = cre->labors;
            df_attrib * physical = cre->physical;

            df_attrib * mental = soul->mental;
            
            c->con << cre;
            c->con << " {" << std::endl << "    ";
            
            c->con << "[" << cre->name.first_name << " " << Tran->TranslateName(&cre->name, false) << "]" << std::endl << "    ";
            
            c->con << "skills: ";
            for (int i=0; i < soul->skills.size(); i++){
                c->con << soul->skills[i]->id << " | ";
            }
            c->con << std::endl << "    ";
            
            c->con << "likes: ";
            for (int i=0; i < soul->likes.size(); i++){
                c->con << soul->likes[i]->type << " | ";
            }
            c->con << std::endl << "    ";

            c->con << "traits: ";
            c->con << soul->traits << " | ";
            c->con << std::endl << "    ";
            
            c->con << "labors: ";
            c->con << labors << " | ";
            c->con << std::endl << "    ";
 
            c->con << "mental: ";
            c->con << mental->unk_0 << " | ";
            c->con << mental->unk_4 << " | ";
            c->con << mental->unk_8 << " | ";
            c->con << mental->unk_c << " | ";
            c->con << mental->unk_10 << " | ";
            c->con << mental->unk_14 << " | ";
            c->con << mental->unk_18 << " | ";
            c->con << std::endl << "    ";
            
            c->con << "physical: ";
            c->con << physical->unk_0 << " | ";
            c->con << physical->unk_4 << " | ";
            c->con << physical->unk_8 << " | ";
            c->con << physical->unk_c << " | ";
            c->con << physical->unk_10 << " | ";
            c->con << physical->unk_14 << " | ";
            c->con << physical->unk_18 << " | ";
            c->con << std::endl << "    ";
            
            c->con << "happiness: ";
            c->con << cre->happiness;
            c->con << "}";
            c->con << std::endl;

        }
    }
}

