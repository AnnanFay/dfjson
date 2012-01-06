//This encodes structs

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

#include "encode.h"

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



// Vectors
template <class VecType>
Value encode_vector(VecType vec){
    Array out;
    for (int i=0; i < vec.size(); i++){
        out.push_back( encode(vec[i]) );
    }
    return Value(out);
}

// Arrays
template <class ArrType>
Value encode_array(ArrType arr, int size){
    return Value("Not Implemented");
}


Value encode(df::unit* rval){
    Object unit;
    unit.push_back(Pair("name", encode(rval->name)));
    unit.push_back(Pair("custom_profession", encode(rval->custom_profession)));
    unit.push_back(Pair("profession", encode(rval->profession)));
    unit.push_back(Pair("profession2", encode(rval->profession2)));
    unit.push_back(Pair("race", encode(rval->race)));
    unit.push_back(Pair("x", encode(rval->x)));
    unit.push_back(Pair("y", encode(rval->y)));
    unit.push_back(Pair("z", encode(rval->z)));
    unit.push_back(Pair("old_x", encode(rval->old_x)));
    unit.push_back(Pair("old_y", encode(rval->old_y)));
    unit.push_back(Pair("old_z", encode(rval->old_z)));
    unit.push_back(Pair("unknown1", encode(rval->unknown1)));
    unit.push_back(Pair("path", encode(rval->path)));
    unit.push_back(Pair("flags1", encode(rval->flags1)));
    unit.push_back(Pair("flags2", encode(rval->flags2)));
    unit.push_back(Pair("flags3", encode(rval->flags3)));
    unit.push_back(Pair("unknown2", encode(rval->unknown2)));
    unit.push_back(Pair("caste", encode(rval->caste)));
    unit.push_back(Pair("sex", encode(rval->sex)));
    unit.push_back(Pair("id", encode(rval->id)));
    unit.push_back(Pair("unk_100", encode(rval->unk_100)));
    unit.push_back(Pair("unk_104", encode(rval->unk_104)));
    unit.push_back(Pair("civ_id", encode(rval->civ_id)));
    unit.push_back(Pair("population_id", encode(rval->population_id)));
    unit.push_back(Pair("unknown3", encode(rval->unknown3)));
    unit.push_back(Pair("meetings", encode(rval->meetings)));
    unit.push_back(Pair("refs", encode(rval->refs)));
    unit.push_back(Pair("military", encode(rval->military)));
    unit.push_back(Pair("unknown4", encode(rval->unknown4)));
    unit.push_back(Pair("mood", encode(rval->mood)));
    unit.push_back(Pair("unk_18e", encode(rval->unk_18e)));
    unit.push_back(Pair("relations", encode(rval->relations)));
    unit.push_back(Pair("last_hit", encode(rval->last_hit)));
    unit.push_back(Pair("inventory", encode(rval->inventory)));
    unit.push_back(Pair("owned_items", encode(rval->owned_items)));
    unit.push_back(Pair("traded_items", encode(rval->traded_items)));
    unit.push_back(Pair("owned_buildings", encode(rval->owned_buildings)));
    unit.push_back(Pair("corpse_parts", encode(rval->corpse_parts)));
    unit.push_back(Pair("job", encode(rval->job)));
    unit.push_back(Pair("body", encode(rval->body)));
    unit.push_back(Pair("unknown6", encode(rval->unknown6)));
    unit.push_back(Pair("appearance", encode(rval->appearance)));
    unit.push_back(Pair("counters", encode(rval->counters)));
    unit.push_back(Pair("status", encode(rval->status)));
    unit.push_back(Pair("hist_figure_id", encode(rval->hist_figure_id)));
    unit.push_back(Pair("status2", encode(rval->status2)));
    unit.push_back(Pair("unknown8", encode(rval->unknown8)));
    unit.push_back(Pair("burrows", encode(rval->burrows)));
    unit.push_back(Pair("unk_93c", encode(rval->unk_93c)));

    return Value(unit);
}

Value encode(std::vector<df::unit*> rval){
    return encode_vector<std::vector<df::unit*>>(rval);
}

Value encode(df::world::T_units rt_units){
    Object t_units;
    t_units.push_back(Pair("all", encode(rt_units.all)));
    return Value(t_units);
}

Value encode(df::world*& rworld){
    Object world;
    world.push_back(Pair("units", encode(rworld->units)));
    return Value(world);
}


// types in unit struct
Value encode(df::unit::T_body rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_counters rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_job rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_last_hit rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_military rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_path rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_relations rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_status rval) {
    Object val;

    val.push_back(Pair("misc_traits", encode(rval.misc_traits)));
    val.push_back(Pair("unk_5b0", encode(rval.unk_5b0)));
    val.push_back(Pair("unk_5b4", encode(rval.unk_5b4)));
    val.push_back(Pair("unk_5b8", encode(rval.unk_5b8)));
    val.push_back(Pair("unk_5bc", encode(rval.unk_5bc)));
    val.push_back(Pair("unk_5cc", encode(rval.unk_5cc)));
    val.push_back(Pair("unk_5dc", encode(rval.unk_5dc)));
    val.push_back(Pair("artifact_name", encode(rval.artifact_name)));
    val.push_back(Pair("souls", encode(rval.souls)));
    val.push_back(Pair("current_soul", encode(rval.current_soul)));
    val.push_back(Pair("unk_660", encode(rval.unk_660)));
    val.push_back(Pair("labors[94]", encode(rval.labors[94])));
    val.push_back(Pair("unk_6d0", encode(rval.unk_6d0)));
    val.push_back(Pair("unk_6e0", encode(rval.unk_6e0)));
    val.push_back(Pair("recent_events", encode(rval.recent_events)));
    val.push_back(Pair("unk_700", encode(rval.unk_700)));
    val.push_back(Pair("happiness", encode(rval.happiness)));
    val.push_back(Pair("unk_714", encode(rval.unk_714)));
    val.push_back(Pair("unk_718", encode(rval.unk_718)));
    val.push_back(Pair("unk_728", encode(rval.unk_728)));
    val.push_back(Pair("relationships", encode(rval.relationships)));
    val.push_back(Pair("unk_748", encode(rval.unk_748)));
    val.push_back(Pair("unk_758", encode(rval.unk_758)));
    val.push_back(Pair("unk_x75a", encode(rval.unk_x75a)));
    val.push_back(Pair("unk_y75c", encode(rval.unk_y75c)));
    val.push_back(Pair("unk_z75e", encode(rval.unk_z75e)));
    val.push_back(Pair("unk_760", encode(rval.unk_760)));
    val.push_back(Pair("unk_770", encode(rval.unk_770)));
    val.push_back(Pair("unk_780", encode(rval.unk_780)));
    return Value(val);
}
Value encode(df::unit::T_status2 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown1 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown2 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown3 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown4 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown6 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit::T_unknown8 rval) {
    return Value("Not Implemented");
}
Value encode(df::language_name rval) {
    Object name;
    name.push_back(Pair("first_name", encode(rval.first_name)));
    name.push_back(Pair("nickname", encode(rval.nickname)));
    name.push_back(Pair("words[7]", encode_array(rval.words, 7)));
    name.push_back(Pair("parts_of_speech[7]", encode_array(rval.parts_of_speech, 7)));
    name.push_back(Pair("language", encode(rval.language)));
    name.push_back(Pair("unknown", encode(rval.unknown)));
    name.push_back(Pair("has_name", encode(rval.has_name)));
    name.push_back(Pair("translated", encode(rval)));
    return Value(name);
}
Value encode(df::unit_flags1 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit_flags2 rval) {
    return Value("Not Implemented");
}
Value encode(df::unit_flags3 rval) {
    return Value("Not Implemented");
}
//Value encode(enum_field<df::profession,int16_t>);

Value encode(df::unit::T_inventory* rval) {
    return Value("Not Implemented");
}
Value encode(df::building* rval) {
    return Value("Not Implemented");
}
Value encode(df::general_ref* rval) {
    return Value("Not Implemented");
}
Value encode(df::meeting_ref* rval) {
    return Value("Not Implemented");
}

// Vectors
Value encode(std::vector<df::unit::T_inventory*> rval) {
    return encode_vector<std::vector<df::unit::T_inventory*>>(rval);
}
Value encode(std::vector<df::building*> rval) {
    return encode_vector<std::vector<df::building*>>(rval);
}
Value encode(std::vector<df::general_ref*> rval) {
    return encode_vector<std::vector<df::general_ref*>>(rval);
}
Value encode(std::vector<df::meeting_ref*> rval) {
    return encode_vector<std::vector<df::meeting_ref*>>(rval);
}
Value encode(std::vector<int32_t> rval) {
    return encode_vector<std::vector<int32_t>>(rval);
}
Value encode(std::vector<uint32_t> rval) {
    return encode_vector<std::vector<uint32_t>>(rval);
}

// Strings
Value encode(std::string rval) {
    return Value(rval);
}

// Ints of varius types
Value encode(int16_t rval) {
    return Value(rval);
}
Value encode(int32_t rval) {
    return Value(rval);
}
Value encode(uint8_t rval) {
    return Value(rval);
}
Value encode(uint16_t rval) {
    return Value(rval);
}
Value encode(uint32_t rval) {
    return Value((long long)rval);
}
