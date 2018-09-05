#ifndef __VDOM__
#define __VDOM__

#include <string>
#include "../core/core_type.h"

using namespace sharpen_type;

namespace sharpen_vdom {

    class Entity {
        std::string vDOMStr;
        TypeRoot* vDOM;

    public:
        Entity (const char* vDOMStr) : vDOMStr(vDOMStr) {
            this->normalize(this->vDOMStr);
        };

        void normalize (const std::string &vDOMStr);
    };

}

#endif
