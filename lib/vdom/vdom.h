#ifndef __VDOM__
#define __VDOM__

#include "../core/core_type.h"
#include "../parser/json.h"
#include <string>

using namespace sharpen_type;
using namespace sharpen_parser;

namespace sharpen_vdom {

    class vDOM {
        Map* vDOMObj;

    public:
        vDOM (std::string vDOMStr) {
            RSJresource jsonRes(vDOMStr);
            this->vDOMObj = static_cast<Map*>(jsonRes.parseAll());
        };
        vDOM (const char* vDOMStr) : vDOM(std::string(vDOMStr))  {};

        Map* getVDOMPtr (void) const {
            return this->vDOMObj;
        }

        Array* to (const vDOM *vl);
        Map* makeCommit (
            int, 
            int, 
            TypeRoot*, 
            TypeRoot*,
            TypeRoot*
        );
        Map* makeCommit (
            int, 
            int, 
            TypeRoot*,
            TypeRoot*
        );
        void _diff (TypeRoot *l, TypeRoot *r, Array* collector);
    };

}

#endif
