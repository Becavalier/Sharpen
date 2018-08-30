#pragma once

#include <string>
#include <vector>
#include <map>

namespace types {

    enum JSTypes {
        JSTYPE_NUMBER = 0,
        JSTYPE_STRING,
        JSTYPE_ARRAY,
        JSTYPE_MAP
    };

    // base type class;
    class TypeRoot {
        JSTypes type;
    public:
        TypeRoot () = default;
        
        // interfaces (pure virtual);
        virtual inline const std::string getPrintData (void) const = 0;

        inline const JSTypes getType (void) const {
            return this->type;
        }
    };

    // type: Number;
    enum JSTypesNumberTypes {
        INTEGER = 0,
        FLOAT
    };

    using numberDataNode = union {
        int i;
        double f;
    };

    // type: String;
    using stringDataNode = std::string;

    // type: Array (Number / String);
    using arrayDataNode = std::vector<TypeRoot*>;

    // type: Map (Object);
    using mapDataNode = std::map<std::string, TypeRoot>;

};
