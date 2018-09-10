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
        char hashPrefix;

        static const bool _hashComp (std::string &sl, std::string &sr);

    public:
        vDOM (std::string vDOMStr, char hashPrefix) : hashPrefix(hashPrefix) {
            RSJresource jsonRes(vDOMStr);
            this->vDOMObj = static_cast<Map*>(jsonRes.parseAll());
        };
        vDOM (const char* vDOMStr, char hashPrefix) : vDOM(std::string(vDOMStr), hashPrefix)  {};

        Map* getVDOMPtr (void) const {
            return this->vDOMObj;
        }

        Array* to (const vDOM *vl);
        // overloads;
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
        Map* makeCommit (
            int, 
            int, 
            TypeRoot*
        );
        void _diff (TypeRoot *l, TypeRoot *r, Array* collector);
    };

}

#endif
