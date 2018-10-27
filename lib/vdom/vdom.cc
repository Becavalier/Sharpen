/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "lib/vdom/vdom.h"
#include "lib/core/core_util.h"
#include "lib/core/core_type_factory.h"

using sharpen_core::Util;

namespace sharpen_vdom {

// VDOMElements (it's not easy to covert a num to "char[]" const expression here in c++);
constexpr char _VDOM_ATTRIBUTES_ [] = "1";
constexpr char _VDOM_CHILDREN_ [] = "2";
constexpr char _VDOM_HASH_ [] = "3";
constexpr char _VDOM_TAGNAME_ [] = "4";
constexpr char _VDOM_TYPE_ [] = "5";
constexpr char _VDOM_INNERTEXT_ [] = "6";
// domTypes;
constexpr int _DOM_TYPE_RELAY_ = 1;
constexpr int _DOM_TYPE_ENDPOINT_ = 2;
constexpr int _DOM_TYPE_EMPTY_ = 3;

// commitActions;
constexpr int _U_ = 1;
constexpr int _C_ = 2;
constexpr int _D_ = 3;
// commitTypes;
constexpr int _HTML_ = 1;
constexpr int _ATTRIBUTE_ = 2;
constexpr int _INNER_TEXT_ = 3;
constexpr int _STYLE_ = 4;
// commitPayload;
constexpr int _CP_ACT_ = 1;
constexpr int _CP_TYP_ = 2;
constexpr int _CP_KEY_ = 3;
constexpr int _CP_HAS_ = 4;
constexpr int _CP_VAL_ = 5;

std::shared_ptr<Map> vDOM::makeCommit(
    int ca,
    int ct,
    std::shared_ptr<TypeRoot> hash,
    std::shared_ptr<TypeRoot> key,
    std::shared_ptr<TypeRoot> value
) {
    std::shared_ptr<Map> m = TypeFactory::buildMap();
    // payload;
    m->addItem(_CP_ACT_, TypeFactory::buildNumber(ca));
    m->addItem(_CP_TYP_, TypeFactory::buildNumber(ct));
    m->addItem(_CP_VAL_, value);
    m->addItem(_CP_KEY_, key);
    m->addItem(_CP_HAS_, hash);

    return m;
}

std::shared_ptr<Map> vDOM::makeCommit(
    int ca,
    int ct,
    std::shared_ptr<TypeRoot> hash,
    std::shared_ptr<TypeRoot> value
) {
    std::shared_ptr<Map> m = TypeFactory::buildMap();
    // payload;
    m->addItem(_CP_ACT_, TypeFactory::buildNumber(ca));
    m->addItem(_CP_TYP_, TypeFactory::buildNumber(ct));
    m->addItem(_CP_VAL_, value);
    m->addItem(_CP_HAS_, hash);

    return m;
}

std::shared_ptr<Map> vDOM::makeCommit(
    int ca,
    int ct,
    std::shared_ptr<TypeRoot> hash
) {
    std::shared_ptr<Map> m = TypeFactory::buildMap();
    // payload;
    m->addItem(_CP_ACT_, TypeFactory::buildNumber(ca));
    m->addItem(_CP_TYP_, TypeFactory::buildNumber(ct));
    m->addItem(_CP_HAS_, hash);

    return m;
}

std::shared_ptr<Array> vDOM::to(const std::shared_ptr<vDOM> v) {
    std::shared_ptr<Map> _t = v->getVDOMPtr();
    std::shared_ptr<Map> _o = this->getVDOMPtr();

    // inti container;
    std::shared_ptr<Array> collector = TypeFactory::buildArray();

    // comparison and commits;
    /**
     * [
     *   {
     *     _CP_ACT_: _D_,
     *     _CP_TYP_: _HTML_,
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
     *     _CP_TYP_: _INNER_TEXT_,
     *     _CP_HAS_: "",
     *     _CP_VAL_: (TypeRoot*)->toJson()
     *   },
     *   {
     *     _CP_ACT_: _U_,
     *     _CP_TYP_: _STYLE_,
     *     _CP_HAS_: "",
     *     _CP_VAL_: (TypeRoot*)->toJson()
     *   }
     * ]
     */

    diff(_o, _t, collector);

    // diff result;
    return collector;
}

mapKeyDataNode vDOM::setOriginalExclude(mapKeyDataNode o, mapKeyDataNode t) {
    mapKeyDataNode i, d;
    std::set_intersection(
        o.begin(),
        o.end(),
        t.begin(),
        t.end(),
        std::back_inserter(i));
    std::set_difference(
        o.begin(),
        o.end(),
        i.begin(),
        i.end(),
        std::back_inserter(d));

    return d;
}

const bool vDOM::hashComp(const std::string &sl, const std::string &sr) {
    return Util::splitDOMSequence(sl) < Util::splitDOMSequence(sr);
}

std::shared_ptr<Map> vDOM::parseKVPair(std::shared_ptr<String> str, char delimiter = ';') {
    std::shared_ptr<Map> map = TypeFactory::buildMap();
    std::istringstream ss(str->getNativeData());
    std::string buffer;
    std::vector<std::string> tokens;

    while (std::getline(ss, buffer, delimiter)) {
        tokens.push_back(buffer);
    }

    for (auto e : tokens) {
        auto pos = e.find(":");
        map->addItem(
            Util::strTrim(e.substr(0, pos)),
            TypeFactory::buildString(
                Util::strTrim(e.substr(pos + 1))));
    }

    return map;
}

void vDOM::diff(
    std::shared_ptr<TypeRoot> o,
    std::shared_ptr<TypeRoot> t,
    std::shared_ptr<Array> collector) {

    // check element type;
    std::shared_ptr<Map> _optr(o->as<Map>());
    std::shared_ptr<Map> _tptr(t->as<Map>());

    // store hash name;
    auto _oHash = _optr->getValue(_VDOM_HASH_);

    // hash equal, test tagName;
    std::shared_ptr<String> oTagName = _optr->getValue(_VDOM_TAGNAME_)->as<String>();
    std::shared_ptr<String> tTagName = _tptr->getValue(_VDOM_TAGNAME_)->as<String>();

    if (!TypeFactory::isEqual(oTagName, tTagName)) {
        collector->addItem(makeCommit(_U_, _HTML_, _oHash, t));
    } else {
        //// attributes; ////
        std::shared_ptr<Map> _oattrsPtr = _optr->getValue(_VDOM_ATTRIBUTES_)->as<Map>();
        std::shared_ptr<Map> _tattrsPtr = _tptr->getValue(_VDOM_ATTRIBUTES_)->as<Map>();

        // get intersections;
        auto _oKeys = _oattrsPtr->getKeyListData();
        auto _tKeys = _tattrsPtr->getKeyListData();

        if (_oKeys.size() != 0 || _tKeys.size() != 0) {
            mapKeyDataNode vKeysDiff = this->setOriginalExclude(
                _oKeys,
                _tKeys);

            // make commits;
            for (auto e : vKeysDiff) {
                collector->addItem(makeCommit(
                    _D_,
                    _ATTRIBUTE_,
                    _oHash,
                    TypeFactory::buildString(e)));
            }

            for (auto attr : _tKeys) {
                auto oVal = _oattrsPtr->getValue(attr);
                auto tVal = _tattrsPtr->getValue(attr);

                // attribute exist :-> String*, otherwise :-> [void]Map*
                if (!(oVal->getType() == JSTypes::JSTYPE_MAP)) {
                    if (!TypeFactory::isEqual(oVal, tVal)) {
                        // update styles;
                        if (attr == "style") {
                            std::shared_ptr<Map> oStyleSheets = this->parseKVPair(oVal->as<String>());
                            std::shared_ptr<Map> tStyleSheets = this->parseKVPair(tVal->as<String>());

                            // get intersections;
                            auto _oStyleKeys = oStyleSheets->getKeyListData();
                            auto _tStyleKeys = tStyleSheets->getKeyListData();

                            mapKeyDataNode vStyleDiff = this->setOriginalExclude(_oStyleKeys, _tStyleKeys);

                            // make commits;
                            for (auto e : vStyleDiff) {
                                collector->addItem(
                                    makeCommit(
                                        _D_,
                                        _STYLE_,
                                        _oHash,
                                        TypeFactory::buildString(e)));
                            }

                            for (auto name : _tStyleKeys) {
                                auto oStyleVal = oStyleSheets->getValue(name);
                                auto tStyleVal = tStyleSheets->getValue(name);

                                if (!(oStyleVal->getType() == JSTypes::JSTYPE_MAP)) {
                                    if (!TypeFactory::isEqual(oStyleVal, tStyleVal)) {
                                        // update style;
                                        collector->addItem(
                                            makeCommit(
                                                _U_,
                                                _STYLE_,
                                                _oHash,
                                                TypeFactory::buildString(name),
                                                tStyleVal));
                                    }
                                } else {
                                    // add style;
                                    collector->addItem(
                                        makeCommit(
                                            _C_,
                                            _STYLE_,
                                            _oHash,
                                            TypeFactory::buildString(name),
                                            tStyleVal));
                                }
                            }

                        } else {
                            // update attr;
                            collector->addItem(
                                makeCommit(
                                    _U_,
                                    _ATTRIBUTE_,
                                    _oHash,
                                    TypeFactory::buildString(attr),
                                    tVal));
                        }
                    }
                } else {
                    // add attr;
                    collector->addItem(
                        makeCommit(
                            _C_,
                            _ATTRIBUTE_,
                            _oHash,
                            TypeFactory::buildString(attr),
                            tVal));
                }
            }
        }

        //// children; ////
        std::shared_ptr<Map> _oChildrenPtr = _optr->getValue(_VDOM_CHILDREN_)->as<Map>();
        std::shared_ptr<Map> _tChildrenPtr = _tptr->getValue(_VDOM_CHILDREN_)->as<Map>();

        // get intersections;
        auto _oChildrenKeys = _oChildrenPtr->getKeyListData();
        auto _tChildrenKeys = _tChildrenPtr->getKeyListData();

        if (_oChildrenKeys.size() == 0 && _tChildrenKeys.size() == 0) {
            // compare tag type and inner text;
            auto _oNodeText = _optr->getValue(_VDOM_INNERTEXT_);
            auto _tNodeText = _tptr->getValue(_VDOM_INNERTEXT_);

            auto _oNodeTextType = _oNodeText->getType();
            auto _tNodeTextType = _tNodeText->getType();

            if ((_oNodeTextType == JSTypes::JSTYPE_STRING) &&
                (_tNodeTextType == JSTypes::JSTYPE_MAP)) {
                collector->addItem(
                    makeCommit(
                        _D_,
                        _INNER_TEXT_,
                        _oHash));
            } else if (
                (_oNodeTextType == JSTypes::JSTYPE_STRING) &&
                (_tNodeTextType == JSTypes::JSTYPE_STRING)) {
                if (!TypeFactory::isEqual(_oNodeText, _tNodeText)) {
                    collector->addItem(
                        makeCommit(
                            _U_,
                            _INNER_TEXT_,
                            _oHash,
                            _tNodeText));
                }
            } else if (
                (_oNodeTextType == JSTypes::JSTYPE_MAP) &&
                (_tNodeTextType == JSTypes::JSTYPE_STRING)) {
                collector->addItem(
                    makeCommit(
                        _C_,
                        _INNER_TEXT_,
                        _oHash,
                        _tNodeText));
            }
        } else {
            mapKeyDataNode vChildrendiff = this->setOriginalExclude(
                _oChildrenKeys,
                _tChildrenKeys,
                this->hashComp);

            // make commits;
            for (auto e : vChildrendiff) {
                collector->addItem(
                    makeCommit(
                        _D_,
                        _HTML_,
                        _oHash));
            }

            for (auto e : _tChildrenKeys) {
                std::string strHashPrefix(1, this->hashPrefix);
                std::string _t = Util::replaceDOMSequence(e, strHashPrefix);

                auto oChildrenVal = _oChildrenPtr->getValue(_t);
                auto tChildrenVal = _tChildrenPtr->getValue(e);

                if (!(oChildrenVal->as<Map>()->getKeyListData().size() == 0)) {
                    this->diff(oChildrenVal, tChildrenVal, collector);
                } else {
                    // add html;
                    collector->addItem(
                        makeCommit(
                            _C_,
                            _HTML_,
                            _oHash,
                            tChildrenVal));
                }
            }
        }
    }
}

}  // namespace sharpen_vdom
