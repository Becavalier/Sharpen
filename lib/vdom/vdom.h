#ifndef __VDOM__
#define __VDOM__

#include "../core/core_type.h"
#include "../parser/json.h"

#include <string>

using namespace sharpen_type;
using namespace sharpen_parser;

using commitActions = enum { _U_ = 1 << 1, _C_, _D_ };
using commitTypes = enum { _HTML_ = 1 << 2, _ATTRIBUTE_ };

namespace sharpen_vdom {

    class vDOM {
        Map* vDOMObj;

    public:
        vDOM (const char* vDOMStr) {
            RSJresource jsonRes(vDOMStr);
            this->vDOMObj = static_cast<Map*>(jsonRes.parseAll());
        };

        Map* getVDOMPtr (void) const {
            return this->vDOMObj;
        }

        Array* to (const vDOM *vl);
        Map* makeCommit (
            commitActions, 
            commitTypes, 
            const char*, 
            const char*,
            const std::string&
        );
        void _diff (const Map *l, const Map *r, Array* collector);
    };

}

#endif
