    // TEMPLATE DEFINITIONS
    /*
        Encoding functions
    */

    /*
        General Case
    */
    
    template <class T>
    json_spirit::Value encode(T & rval) {
        // Don't assume that it can be encoded by something else, just pass back nothing
        return typeid(rval).name();
    }

    /*
        Pointers
    */
    template <class T>
    json_spirit::Value encode(T* rval) {
        // deref and encode
        return encode(*rval);
    }

    // A pointer to an unknown
    json_spirit::Value encode(void* rval);

    /*
        Primitives - Integers
    */

    // must be cast tobool, int, boost::int64_t, boost::uint64_t(long long) or double

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

    /*
        Primitives - Arrays
    */

    template <class T, std::size_t N>
    json_spirit::Value encode(T (&rval)[N]){
        json_spirit::Array out;
        for (int i=0; i < N; i++){
            out.push_back( encode(rval[i]) );
        }
        return json_spirit::Value(out);
    }

    /*
        STD Types
    */

    json_spirit::Value encode(std::string & rval);

    template<class T>
    json_spirit::Value encode(std::vector<T> rval) {
        json_spirit::Array out;
        for (int i=0; i < rval.size(); i++){
            out.push_back( encode(rval[i]) );
        }
        return json_spirit::Value(out);
    }


    /*
        DF Types
    */
    template<class EnumType, class IntType>
    json_spirit::Value encode(df::enum_field<EnumType,IntType> rval) {
        return json_spirit::Value();
    }


}

#endif