/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_VDOM_VDOM_H_
#define LIB_VDOM_VDOM_H_

#include <string>
#include "lib/core/core_type.h"
#include "lib/parser/json.h"

using sharpen_type::TypeRoot;
using sharpen_type::mapKeyDataNode;
using sharpen_core::String;
using sharpen_parser::RSJresource;

namespace sharpen_vdom {

class vDOM {
    Map* vDOMObj;
    char hashPrefix;

    static const bool hashComp(const std::string &sl, const std::string &sr);
    void diff(TypeRoot *l, TypeRoot *r, Array* collector);
    Map* parseKVPair(String*, char);
    // overloads;
    Map* makeCommit(
        int,
        int,
        TypeRoot*,
        TypeRoot*,
        TypeRoot*);
    Map* makeCommit(
        int,
        int,
        TypeRoot*,
        TypeRoot*);
    Map* makeCommit(
        int,
        int,
        TypeRoot*);

    Map* getVDOMPtr(void) const {
        return this->vDOMObj;
    }

    mapKeyDataNode setOriginalExclude(mapKeyDataNode o, mapKeyDataNode t);

    template<typename Compare>
    mapKeyDataNode setOriginalExclude(mapKeyDataNode o, mapKeyDataNode t, Compare comp) {
        mapKeyDataNode i, d;
        std::set_intersection(
            o.begin(),
            o.end(),
            t.begin(),
            t.end(),
            std::back_inserter(i),
            comp);
        std::set_difference(
            o.begin(),
            o.end(),
            i.begin(),
            i.end(),
            std::back_inserter(d),
            comp);

        return d;
    }

 public:
    vDOM(std::string vDOMStr, char hashPrefix) : hashPrefix(hashPrefix) {
        RSJresource jsonRes(vDOMStr);
        this->vDOMObj = static_cast<Map*>(jsonRes.parseAll());
    }
    vDOM(const char* vDOMStr, char hashPrefix) : vDOM(std::string(vDOMStr), hashPrefix) {}
    Array* to(const vDOM *vl);
};

}  // namespace sharpen_vdom

#endif  // LIB_VDOM_VDOM_H_
