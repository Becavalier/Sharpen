#ifndef __CORE_TYPES__
#define __CORE_TYPES__

#include <string>
#include <vector>
#include <map>

namespace sharpen_type {

    enum JSTypes {
        JSTYPE_INTEGER = 0,
        JSTYPE_FLOAT,
        JSTYPE_STRING,
        JSTYPE_BOOL,
        JSTYPE_ARRAY,
        JSTYPE_MAP
    };

    // base type class;
    class TypeRoot {
        JSTypes type;
    public:
        TypeRoot (JSTypes JSType) : type(JSType) {};
        
        // interfaces (pure virtual);
        virtual inline const std::string getPrintData (void) const = 0;

        inline const JSTypes getType (void) const {
            return this->type;
        }
    };

    using numberDataNode = union {
        int i;
        double f;
    };

    // type: Bool;
    using boolDataNode = bool;

    // type: String;
    using stringDataNode = std::string;

    // type: Array (Number / String);
    using arrayDataNode = std::vector<TypeRoot*>;

    // type: Map (Object);
    using mapDataNode = std::map<std::string, TypeRoot*>;
    using mapKeyDataNode = std::vector<std::string>;
    using mapDataNodeSingle = std::pair<std::string, TypeRoot*>;
};

#endif
