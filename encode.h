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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace DFHack;
using namespace json_spirit;
using namespace std;
using namespace boost;

// types in unit struct
Value encode(df::unit::T_body);
Value encode(df::unit::T_counters);
Value encode(df::unit::T_job);
Value encode(df::unit::T_last_hit);
Value encode(df::unit::T_military);
Value encode(df::unit::T_path);
Value encode(df::unit::T_relations);
Value encode(df::unit::T_status);
Value encode(df::unit::T_status2);
Value encode(df::unit::T_unknown1);
Value encode(df::unit::T_unknown2);
Value encode(df::unit::T_unknown3);
Value encode(df::unit::T_unknown4);
Value encode(df::unit::T_unknown6);
Value encode(df::unit::T_unknown8);
Value encode(df::language_name);
Value encode(df::unit_flags1);
Value encode(df::unit_flags2);
Value encode(df::unit_flags3);
//Value encode(enum_field<df::profession,int16_t>);


Value encode(df::unit::T_inventory* rval);
Value encode(df::building* rval);
Value encode(df::general_ref* rval);
Value encode(df::meeting_ref* rval);


Value encode(std::string);
Value encode(std::vector<df::unit::T_inventory*>);
Value encode(std::vector<df::building*>);
Value encode(std::vector<df::general_ref*>);
Value encode(std::vector<df::meeting_ref*>);
Value encode(std::vector<int32_t>);
Value encode(std::vector<uint32_t>);

Value encode(int8_t);
Value encode(int16_t);
Value encode(int32_t);
Value encode(uint8_t);
Value encode(uint16_t);
Value encode(uint32_t);

// Others
Value encode(df::unit* unit);
Value encode(std::vector<df::unit*>);
Value encode(df::world::T_units);
Value encode(df::world*&);


//New 
Value encode(df::unit::T_body);
Value encode(df::unit::T_counters);
Value encode(df::unit::T_job);
Value encode(df::unit::T_last_hit);
Value encode(df::unit::T_military);
Value encode(df::unit::T_path);
Value encode(df::unit::T_military::T_pickup_flags);
Value encode(df::unit::T_relations);
Value encode(df::unit::T_status);
Value encode(df::unit::T_status2);
Value encode(df::unit::T_status::T_unk_5b0*);
Value encode(df::unit::T_unknown1);
Value encode(df::unit::T_unknown2);
Value encode(df::unit::T_unknown3);
Value encode(df::unit::T_unknown4);
Value encode(df::unit::T_unknown6);
Value encode(df::unit::T_unknown8);
Value encode(bool);
Value encode(char);
Value encode(df::building*);
Value encode(df::item*);
Value encode(df::job*);
Value encode(df::language_name);
Value encode(df::unit*);
Value encode(df::unit_attribute);
Value encode(df::unit_flags1);
Value encode(df::unit_flags2);
Value encode(df::unit_flags3);
Value encode(df::enum_field<df::unit::T_counters::T_soldier_mood,int16_t>);
Value encode(df::enum_field<df::item_type,int16_t>);
Value encode(df::enum_field<df::profession,int16_t>);
Value encode(std::vector<df::unit::T_status2::T_body_part_temperature*>);
Value encode(std::vector<df::unit::T_inventory*>);
Value encode(std::vector<df::body_part_raw*>*);
Value encode(std::vector<df::building*>);
Value encode(std::vector<df::general_ref*>);
Value encode(std::vector<df::meeting_ref*>);
Value encode(std::vector<df::unit_misc_trait*>);
Value encode(std::vector<int16_t>);
Value encode(std::vector<int32_t>);
Value encode(std::vector<uint16_t>);
Value encode(std::vector<uint32_t>);
Value encode(std::vector<uint8_t>);
Value encode(std::vector<void*>);
Value encode(std::vector<df::unit_soul*>&);
