/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_VDOM_VDOM_H_
#define LIB_VDOM_VDOM_H_

#include <string>
#include <memory>
#include "lib/core/core_type.h"
#include "lib/parser/json.h"

using sharpen_type::TypeRoot;
using sharpen_type::mapKeyDataNode;
using sharpen_core::String;
using sharpen_parser::Json;

namespace sharpen_vdom {

class vDOM {
    std::shared_ptr<Map> vDOMObj;
    char hashPrefix;

    static const bool hashComp(const std::string &sl, const std::string &sr);
    void diff(std::shared_ptr<TypeRoot>, std::shared_ptr<TypeRoot>, std::shared_ptr<Array>);
    std::shared_ptr<Map> parseKVPair(std::shared_ptr<String>, char);
    // overloads;
    std::shared_ptr<Map> makeCommit(
        int,
        int,
        std::shared_ptr<TypeRoot>,
        std::shared_ptr<TypeRoot>,
        std::shared_ptr<TypeRoot>);
    std::shared_ptr<Map> makeCommit(
        int,
        int,
        std::shared_ptr<TypeRoot>,
        std::shared_ptr<TypeRoot>);
    std::shared_ptr<Map> makeCommit(
        int,
        int,
        std::shared_ptr<TypeRoot>);

    std::shared_ptr<Map> getVDOMPtr(void) const {
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
        Json jsonRes(vDOMStr);
        this->vDOMObj = std::static_pointer_cast<Map>(jsonRes.parseAll());
    }
    vDOM(const char* vDOMStr, char hashPrefix) : vDOM(std::string(vDOMStr), hashPrefix) {}
    std::shared_ptr<Array> to(const std::shared_ptr<vDOM> vl);
};

}  // namespace sharpen_vdom

#endif  // LIB_VDOM_VDOM_H_
