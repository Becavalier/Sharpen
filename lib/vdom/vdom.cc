#include "vdom.h"
#include "../core/core_type_factory.h"

#include <iostream>

using namespace std;

namespace sharpen_vdom {

    Map* vDOM::makeCommit (
        commitActions ca, 
        commitTypes ct, 
        const char* key, 
        const char* hash, 
        const std::string& value
    ) {
        Map* m = TypeFactory::buildMap();
        // payload;
        m->addItem("action", TypeFactory::buildNumber(ca));
        m->addItem("type", TypeFactory::buildNumber(ct));
        m->addItem("key", TypeFactory::buildString(key));
        m->addItem("hash", TypeFactory::buildString(hash));
        m->addItem("value", TypeFactory::buildString(value));

        return m;
    }

    Array* vDOM::to (const vDOM *v) {
        Map* _t = v->getVDOMPtr();
        Map* _o = this->getVDOMPtr();

        // inti container;
        Array* collector = TypeFactory::buildArray();

        // comparison and commits;
        /**
         * [
         *   {
         *     action: enum(_D_),
         *     type: "html",
         *     key: "",
         *     hash: "",
         *     value: json[TypeRoot*]
         *   },
         *   {
         *     action: enum(_U_),
         *     type: "attr",
         *     key: "",
         *     hash: "",
         *     value: json[TypeRoot*]
         *   },
         *   {
         *     action: enum(_C_),
         *     type: "attr",
         *     key: "",
         *     hash: "",
         *     value: json[TypeRoot*]
         *   }
         * ]
         */

        _diff(_o, _t, collector);
    }

    void vDOM::_diff (const Map *l, const Map *r, Array* collector) {
        // check element type;
        /*
        if (l->getValue("tagName") != r->getValue("tagName")) {
            collector->addItem(makeCommit())
        }
        */
    }   

}
