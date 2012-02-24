#include "encode.h"

namespace dfjson
{


    /*
        Encoding functions
    */

    /*
        Pointers
    */

    // A pointer to an unknown
    js::Value encode(jsmap & json, void* rval) {
        return js::Value();
    }

    /*
        Primitives - Integers
    */

    // must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double

    js::Value encode(jsmap & json, bool & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, int rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, long & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(jsmap & json, long long & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, short & rval){
        return js::Value((int)rval);
    }
    js::Value encode(jsmap & json, signed char & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned char & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned int & rval){
        return js::Value((long long)rval);
    }
    js::Value encode(jsmap & json, unsigned long & rval){
        return js::Value((unsigned long long)rval);
    }
    js::Value encode(jsmap & json, unsigned long long & rval){
        return js::Value(rval);
    }
    js::Value encode(jsmap & json, unsigned short & rval){
        return js::Value(rval);
    }

    /*
        STD Types
    */

    js::Value encode(jsmap & json, std::string & rval) {
        log(json, &rval, rval);
        return js::Value(rval);
    }



    js::Value translate(jsmap & json, df::language_name * name){
        
        df::language_translation * trans = world->raws.translations[name->language];

        std::vector<std::string> translated;

        for (int i=0; i < 7;i++){
            if (name->words[i] > -1){
                translated.push_back(*trans->words[name->words[i]]);
            }
        }

        return encode(json, translated);
    }

    js::Value get_race(jsmap & json, df::unit * unit){
        return encode(json, world->raws.creatures.all[unit->race]->creature_id);
    }


    js::Value encode(jsmap & json, df::creature_raw* rval) {
        log(json, rval);
        if(!rval){return Value();};
        std::string p = global_exists(json, rval);
        if (p != ""){ return js::Value(p);}
        js::Object val;
        val.push_back(Pair("creature_id",                         encode(json, rval->creature_id)));                         //std::string
        //val.push_back(Pair("name",                                encode_array(json, rval->name, 3)));                                //std::string[]
        //val.push_back(Pair("general_baby_name",                   encode_array(json, rval->general_baby_name, 2)));                   //std::string[]
        //val.push_back(Pair("general_child_name",                  encode_array(json, rval->general_child_name, 2)));                  //std::string[]
        val.push_back(Pair("creature_tile",                       encode(json, rval->creature_tile)));                       //uint8_t
        val.push_back(Pair("creature_soldier_tile",               encode(json, rval->creature_soldier_tile)));               //uint8_t
        val.push_back(Pair("alttile",                             encode(json, rval->alttile)));                             //uint8_t
        val.push_back(Pair("soldier_alttile",                     encode(json, rval->soldier_alttile)));                     //uint8_t
        val.push_back(Pair("glowtile",                            encode(json, rval->glowtile)));                            //uint8_t
        val.push_back(Pair("temperature1",                        encode(json, rval->temperature1)));                        //uint16_t
        val.push_back(Pair("temperature2",                        encode(json, rval->temperature2)));                        //uint16_t
        val.push_back(Pair("frequency",                           encode(json, rval->frequency)));                           //int16_t
        val.push_back(Pair("population_number",                   encode_array(json, rval->population_number, 2)));                   //int16_t[]
        val.push_back(Pair("cluster_number",                      encode_array(json, rval->cluster_number, 2)));                      //int16_t[]
        val.push_back(Pair("triggerable_group",                   encode_array(json, rval->triggerable_group, 2)));                   //int16_t[]
        val.push_back(Pair("color",                               encode_array(json, rval->color, 3)));                               //int16_t[]
        val.push_back(Pair("glowcolor",                           encode_array(json, rval->glowcolor, 3)));                           //int16_t[]
        val.push_back(Pair("adultsize",                           encode(json, rval->adultsize)));                           //int32_t
        //val.push_back(Pair("prefstring",                          encode(json, rval->prefstring)));                          //std::vector<std::string*>
        val.push_back(Pair("sphere",                              encode(json, rval->sphere)));                              //std::vector<int16_t>
        //val.push_back(Pair("caste",                               encode(json, rval->caste)));                               //std::vector<df::caste_raw*>
        val.push_back(Pair("pop_ratio",                           encode(json, rval->pop_ratio)));                           //std::vector<int32_t>
        val.push_back(Pair("flags",                               encode(json, rval->flags)));                               //BitArray<NOT IMPLEMENTED>
        val.push_back(Pair("stuff",                               encode(json, rval->stuff)));                               //df::creature_raw::T_stuff
        val.push_back(Pair("unk5",                                encode(json, rval->unk5)));                                //std::vector<void*>
        val.push_back(Pair("speech1",                             encode(json, rval->speech1)));                             //std::vector<void*>
        val.push_back(Pair("speech2",                             encode(json, rval->speech2)));                             //std::vector<void*>
        val.push_back(Pair("speech3",                             encode(json, rval->speech3)));                             //std::vector<void*>
        //val.push_back(Pair("material",                            encode(json, rval->material)));                            //std::vector<df::material*>
        val.push_back(Pair("tissue",                              encode(json, rval->tissue)));                              //std::vector<void**>
        val.push_back(Pair("profession_name",                     encode_array(json, rval->profession_name, 106)));                     //df::creature_raw::T_profession_name[]
        val.push_back(Pair("unk6pa",                              encode(json, rval->unk6pa)));                              //void**
        val.push_back(Pair("unk6pb",                              encode(json, rval->unk6pb)));                              //void**
        val.push_back(Pair("unk6",                                encode(json, rval->unk6)));                                //std::vector<int32_t>
        val.push_back(Pair("unk7",                                encode(json, rval->unk7)));                                //std::vector<int32_t>
        val.push_back(Pair("hive_product_0",                      encode(json, rval->hive_product_0)));                      //std::vector<int32_t>
        val.push_back(Pair("hive_product_1",                      encode(json, rval->hive_product_1)));                      //std::vector<int32_t>
        val.push_back(Pair("hive_product_2",                      encode(json, rval->hive_product_2)));                      //std::vector<int16_t>
        val.push_back(Pair("hive_product_3",                      encode(json, rval->hive_product_3)));                      //std::vector<int16_t>
        val.push_back(Pair("hive_product_4",                      encode(json, rval->hive_product_4)));                      //std::vector<int16_t>
        val.push_back(Pair("hive_product_5",                      encode(json, rval->hive_product_5)));                      //std::vector<int32_t>
        //val.push_back(Pair("hive_product_tmpstr",                 encode_array(json, rval->hive_product_tmpstr, 5)));                 //std::vector<std::string*>[]
        val.push_back(Pair("unk8",                                encode(json, rval->unk8)));                                //int32_t
        //val.push_back(Pair("raws",                                encode(json, rval->raws)));                                //std::vector<std::string*>
        return js::Value(global_add(json, rval, val));
    }
    js::Value encode(jsmap & json, df::creature_raw& rval){return encode(json, &rval);}


}