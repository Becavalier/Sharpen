#include "vdom.h"
#include "../core/core_util.h"
#include "../core/core_type_factory.h"
#include <iostream>
#include <algorithm>
#include <iterator>

namespace sharpen_vdom {

    // commitActions;
    int _U_ = 1 << 1;  // 2;
    int _C_ = 1 << 2;  // 4;
    int _D_ = 1 << 3;  // 8;

    // commitTypes;
    int _HTML_ = 1 << 1;  // 2;
    int _ATTRIBUTE_ = 1 << 2;  // 4;

    // commitPayload;
    int _CP_ACT_ = 1 << 1;  // 2;
    int _CP_TYP_ = 1 << 2;  // 4;
    int _CP_KEY_ = 1 << 3;  // 8;
    int _CP_HAS_ = 1 << 4;  // 16;
    int _CP_VAL_ = 1 << 5;  // 32;

    
    Map* vDOM::makeCommit (
        int ca, 
        int ct, 
        TypeRoot* hash, 
        TypeRoot* key,
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
        try {
            // check element type;
            Map* _optr(static_cast<Map*>(o));
            Map* _tptr(static_cast<Map*>(t));

            if (!TypeFactory::isEqual(_optr->getValue("tagName"), _tptr->getValue("tagName"))) {
                collector->addItem(makeCommit(_U_, _HTML_, _optr->getValue("hash"), o));
            } else {
                //// attributes; ////
                Map* _oattrsPtr = static_cast<Map*>(_optr->getValue("attributes"));
                Map* _tattrsPtr = static_cast<Map*>(_tptr->getValue("attributes"));

                // get intersections;
                auto _oKeys = _oattrsPtr->getKeyListData();
                auto _tKeys = _tattrsPtr->getKeyListData();    

                if (_oKeys.size() != 0 || _tKeys.size() != 0) {
                    mapKeyDataNode vint;
                    std::set_intersection(
                        _oKeys.begin(), 
                        _oKeys.end(), 
                        _tKeys.begin(), 
                        _tKeys.end(), 
                        std::back_inserter(vint)
                    );

                    // get difference(exclude);
                    mapKeyDataNode vdiff;
                    std::set_difference(
                        _oKeys.begin(), 
                        _oKeys.end(), 
                        vint.begin(), 
                        vint.end(), 
                        std::back_inserter(vdiff)
                    );
                    
                    // make commits;
                    for (auto e : vdiff) {
                        collector->addItem(makeCommit(_D_, _ATTRIBUTE_, _optr->getValue("hash"), TypeFactory::buildString(e)));
                    }

                    for (auto e : _tKeys) {
                        auto oVal = _oattrsPtr->getValue(e);
                        auto tVal = _tattrsPtr->getValue(e);
                        if (!TypeFactory::isEqual(oVal, TypeFactory::buildBool(false))) {
                            if (oVal != tVal) {
                                // update attr;
                                collector->addItem(
                                    makeCommit(_U_, _ATTRIBUTE_, _optr->getValue("hash"), TypeFactory::buildString(e), tVal));
                            }
                        } else {
                            // add attr;
                            collector->addItem(
                                makeCommit(_C_, _ATTRIBUTE_, _optr->getValue("hash"), TypeFactory::buildString(e), tVal));
                        }
                    }
                }

                //// children; ////
                Map* _oChildrenPtr = static_cast<Map*>(_optr->getValue("children"));
                Map* _tChildrenPtr = static_cast<Map*>(_tptr->getValue("children"));

                // get intersections;
                auto _oChildrenKeys = _oChildrenPtr->getKeyListData();
                auto _tChildrenKeys = _tChildrenPtr->getKeyListData();               
                
                if (_oChildrenKeys.size() == 0 && _tChildrenKeys.size() == 0) 
                    return; 
                
                mapKeyDataNode vChildrenint;
                std::set_intersection(
                    _oChildrenKeys.begin(), 
                    _oChildrenKeys.end(), 
                    _tChildrenKeys.begin(), 
                    _tChildrenKeys.end(), 
                    std::back_inserter(vChildrenint)
                );

                // get difference(exclude);
                mapKeyDataNode vChildrendiff;
                std::set_difference(
                    _oChildrenKeys.begin(), 
                    _oChildrenKeys.end(), 
                    vChildrenint.begin(), 
                    vChildrenint.end(), 
                    std::back_inserter(vChildrendiff)
                );
                
                // make commits;
                for (auto e : vChildrendiff) {
                    collector->addItem(makeCommit(_D_, _HTML_, _optr->getValue("hash"), TypeFactory::buildString(e)));
                }

                for (auto e : _tChildrenKeys) {
                    auto oChildrenVal = _oChildrenPtr->getValue(e);
                    auto tChildrenVal = _tChildrenPtr->getValue(e);

                    if (!TypeFactory::isEqual(oChildrenVal, TypeFactory::buildBool(false))) {
                        this->_diff(oChildrenVal, tChildrenVal, collector);
                    } else {
                        // add html;
                        collector->addItem(
                            makeCommit(_C_, _HTML_, _optr->getValue("hash"), tChildrenVal));
                    }
                }   
            }
        } catch (std::exception& e) {
            std::cerr << "exception caught: " << e.what() << std::endl;
        } 
    } 
}
