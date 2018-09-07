#include "vdom.h"
#include "../core/core_util.h"
#include "../core/core_type_factory.h"
#include <iostream>

namespace sharpen_vdom {

    // commitActions;
    int _U_ = 1 << 1;  // 2;
    int _C_ = 1 << 2;  // 4;
    int _D_ = 1 << 3;  // 8;

    // commitTypes;
    int _HTML_ = 1 << 4;  // 16;
    int _ATTRIBUTE_ = 1 << 5;  // 32;

    // commitPayload;
    int _CP_ACT_ = 1 << 6;  // 64;
    int _CP_TYP_ = 1 << 7;  // 128;
    int _CP_KEY_ = 1 << 8;  // 256;
    int _CP_HAS_ = 1 << 9;  // 512;
    int _CP_VAL_ = 1 << 10;  // 1024;

    
    Map* vDOM::makeCommit (
        int ca, 
        int ct, 
        TypeRoot* key, 
        TypeRoot* hash, 
        TypeRoot* value
    ) {
        Map* m = TypeFactory::buildMap();
        // payload;
        m->addItem(_CP_ACT_, TypeFactory::buildNumber(ca));
        m->addItem(_CP_TYP_, TypeFactory::buildNumber(ct));
        m->addItem(_CP_VAL_, value);
        m->addItem(_CP_KEY_, key);
        m->addItem(_CP_HAS_, hash);
        
        return m;
    }

    Map* vDOM::makeCommit (
        int ca, 
        int ct, 
        TypeRoot* hash, 
        TypeRoot* value
    ) {
        Map* m = TypeFactory::buildMap();
        // payload;
        m->addItem(_CP_ACT_, TypeFactory::buildNumber(ca));
        m->addItem(_CP_TYP_, TypeFactory::buildNumber(ct));
        m->addItem(_CP_VAL_, value);
        m->addItem(_CP_HAS_, hash);
        
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
         *     _CP_ACT_: _D_,
         *     _CP_TYP_: _HTML_,
         *     _CP_KEY_: "",
         *     _CP_HAS_: "",
         *     _CP_VAL_: (TypeRoot*)->toJson()
         *   },
         *   {
         *     _CP_ACT_: _U_,
         *     _CP_TYP_: _ATTR_,
         *     _CP_KEY_: "",
         *     _CP_HAS_: "",
         *     _CP_VAL_: (TypeRoot*)->toJson()
         *   },
         *   {
         *     _CP_ACT_: _C_,
         *     _CP_TYP_: _ATTR_,
         *     _CP_KEY_: "",
         *     _CP_HAS_: "",
         *     _CP_VAL_: (TypeRoot*)->toJson()
         *   }
         * ]
         */

        _diff(_o, _t, collector);

        // diff result;
        return collector;
    }

    void vDOM::_diff (TypeRoot *o, TypeRoot *t, Array* collector) {
        // check element type;
        if (!TypeFactory::isEqual(static_cast<Map*>(o)->getValue("tagName"), static_cast<Map*>(t)->getValue("tagName"))) {
            collector->addItem(makeCommit(_U_, _HTML_, static_cast<Map*>(o)->getValue("hash"), t));
        }
    }   

}
