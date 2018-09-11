/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_VDOM_VDOM_H_
#define LIB_VDOM_VDOM_H_

#include <string>
#include "lib/core/core_type.h"
#include "lib/parser/json.h"

using sharpen_type::TypeRoot;
using sharpen_core::String;
using sharpen_parser::RSJresource;

namespace sharpen_vdom {

class vDOM {
    Map* vDOMObj;
    char hashPrefix;

    static const bool _hashComp(const std::string &sl, const std::string &sr);

 public:
    vDOM(std::string vDOMStr, char hashPrefix) : hashPrefix(hashPrefix) {
        RSJresource jsonRes(vDOMStr);
        this->vDOMObj = static_cast<Map*>(jsonRes.parseAll());
    }
    vDOM(const char* vDOMStr, char hashPrefix) : vDOM(std::string(vDOMStr), hashPrefix) {}

    Map* getVDOMPtr(void) const {
        return this->vDOMObj;
    }

    Array* to(const vDOM *vl);

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

    void _diff(TypeRoot *l, TypeRoot *r, Array* collector);
};

}  // namespace sharpen_vdom

#endif  // LIB_VDOM_VDOM_H_
