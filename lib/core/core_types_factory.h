#ifndef __CORE_TYPES_FACTORY__
#define __CORE_TYPES_FACTORY__

#include "core_types_class.h"

#include <string>

using namespace types;

namespace core {

    class TypeFactory {
        template<typename T>
        static T* downCastP (TypeRoot* t);

    public:
        TypeFactory () = default;

        // factory methods;
        inline Number<double>* buildNumber (double t) {
            return new Number<double>(t);
        }

        inline Number<int>* buildNumber (int t) {
            return new Number<int>(t);
        }

        inline Bool* buildBool (bool t) {
            return new Bool(t);
        }

        inline String* buildString (const std::string &s) {
            return new String(s);
        }

        inline Array* buildArray () {
            return new Array();
        }

        inline Map* buildMap () {
            return new Map();
        }

        static bool isEqual (TypeRoot*, TypeRoot*);
    };
    
}

#endif
