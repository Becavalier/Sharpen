#ifndef __CORE_TYPES_FACTORY__
#define __CORE_TYPES_FACTORY__

#include "core_type_class.h"

#include <string>

using namespace sharpen_type;

namespace sharpen_core {

    class TypeFactory {
        template<typename T>
        static T* downCastP (TypeRoot* t);

    public:
        TypeFactory () = default;
        ~TypeFactory () = default;

        // factory methods;
        static inline Number<double>* buildNumber (double t) {
            return new Number<double>(t);
        }

        static inline Number<int>* buildNumber (int t) {
            return new Number<int>(t);
        }

        static inline Bool* buildBool (bool t) {
            return new Bool(t);
        }

        static inline String* buildString (const std::string &s) {
            return new String(s);
        }

        static inline Array* buildArray () {
            return new Array();
        }

        static inline Map* buildMap () {
            return new Map();
        }

        static bool isEqual (TypeRoot*, TypeRoot*);
    };
    
}

#endif
