#ifndef DF_ENCODE_H
#define DF_ENCODE_H

#include <vector>
#include <string>
#include <typeinfo>

#include <json_spirit/json_spirit_reader_template.h>
#include <json_spirit/json_spirit_writer_template.h>

#include <DataDefs.h>

#include <df-headers.h>


// Are these all needed?
#include <DFHack.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>

using namespace std;
using namespace json_spirit;
namespace js = json_spirit;
using namespace boost;
using namespace DFHack;


namespace dfjson
{
    // DECLARATIONS

    // Templates
    template <class T>                          json_spirit::Value encode(T & rval);
    template <class T>                          json_spirit::Value encode(T* rval);
    template <class T, std::size_t N>           json_spirit::Value encode(T (&rval)[N]);
    template <class T>                          json_spirit::Value encode(std::vector<T> rval);
    template <class EnumType, class IntType>    json_spirit::Value encode(df::enum_field<EnumType,IntType> rval);

    // Functions
    json_spirit::Value encode(bool & rval);
    json_spirit::Value encode(int rval);
    json_spirit::Value encode(long & rval);
    json_spirit::Value encode(long long & rval);
    json_spirit::Value encode(short & rval);
    json_spirit::Value encode(signed char & rval);
    json_spirit::Value encode(unsigned char & rval);
    json_spirit::Value encode(unsigned int & rval);
    json_spirit::Value encode(unsigned long & rval);
    json_spirit::Value encode(unsigned long long & rval);
    json_spirit::Value encode(unsigned short & rval);
    json_spirit::Value encode(std::string & rval);
    json_spirit::Value encode(void* rval);