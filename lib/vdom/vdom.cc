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

// commitActions;
int _U_ = 1 << 1;  // 2;
int _C_ = 1 << 2;  // 4;
int _D_ = 1 << 3;  // 8;

// commitTypes;
int _HTML_ = 1 << 1;  // 2;
int _ATTRIBUTE_ = 1 << 2;  // 4;
int _INNER_TEXT_ = 1 << 3;  // 8;
int _STYLE_ = 1 << 4;  // 16;

// commitPayload;
int _CP_ACT_ = 1 << 1;  // 2;
int _CP_TYP_ = 1 << 2;  // 4;
int _CP_KEY_ = 1 << 3;  // 8;
int _CP_HAS_ = 1 << 4;  // 16;
int _CP_VAL_ = 1 << 5;  // 32;

// domTypes;
int _DOM_TYPE_RELAY_ = 1 << 1;  // 2;
int _DOM_TYPE_ENDPOINT_ = 1 << 2;  // 4;
int _DOM_TYPE_EMPTY_ = 1 << 3;  // 8;

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
    return TypeFactory::splitStr(sl) < TypeFactory::splitStr(sr);
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
            Util::strtrim(e.substr(0, pos)),
            TypeFactory::buildString(
                Util::strtrim(e.substr(pos + 1))));
    }

    return map;
}

void vDOM::diff(
    std::shared_ptr<TypeRoot> o,
    std::shared_ptr<TypeRoot> t,
    std::shared_ptr<Array> collector) {
    try {
        // check element type;
        std::shared_ptr<Map> _optr(o->as<Map>());
        std::shared_ptr<Map> _tptr(t->as<Map>());

        // store hash name;
        auto _oHash = _optr->getValue("hash");

        // hash equal, test tagName;
        std::shared_ptr<String> oTagName = _optr->getValue("tagName")->as<String>();
        std::shared_ptr<String> tTagName = _tptr->getValue("tagName")->as<String>();

        if (!TypeFactory::isEqual(oTagName, tTagName)) {
            collector->addItem(makeCommit(_U_, _HTML_, _oHash, t));
        } else {
            //// attributes; ////
            std::shared_ptr<Map> _oattrsPtr = _optr->getValue("attributes")->as<Map>();
            std::shared_ptr<Map> _tattrsPtr = _tptr->getValue("attributes")->as<Map>();

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
            std::shared_ptr<Map> _oChildrenPtr = _optr->getValue("children")->as<Map>();
            std::shared_ptr<Map> _tChildrenPtr = _tptr->getValue("children")->as<Map>();

            // get intersections;
            auto _oChildrenKeys = _oChildrenPtr->getKeyListData();
            auto _tChildrenKeys = _tChildrenPtr->getKeyListData();

            if (_oChildrenKeys.size() == 0 && _tChildrenKeys.size() == 0) {
                // compare tag type and inner text;
                auto _oNodeText = _optr->getValue("innerText");
                auto _tNodeText = _tptr->getValue("innerText");

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
                    std::string _t =  TypeFactory::replaceStr(e, strHashPrefix);

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
    } catch (std::exception& e) {
        std::cerr << "exception caught: " << e.what() << std::endl;
    }
}

}  // namespace sharpen_vdom
