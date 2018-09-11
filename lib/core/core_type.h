/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_TYPE_H_
#define LIB_CORE_CORE_TYPE_H_

#include <string>
#include <vector>
#include <map>
#include <utility>

namespace sharpen_type {

enum JSTypes {
    JSTYPE_INTEGER = 0,
    JSTYPE_FLOAT,
    JSTYPE_STRING,
    JSTYPE_BOOL,
    JSTYPE_ARRAY,
    JSTYPE_MAP
};

// base type class;
class TypeRoot {
    JSTypes type;
 public:
    explicit TypeRoot(JSTypes JSType) : type(JSType) {}
    ~TypeRoot() = default;

    // interfaces (pure virtual);
    virtual const std::string toJson(void) const = 0;

    const JSTypes getType(void) const {
        return this->type;
    }
};

using numberDataNode = union {
    int i;
    double f;
};

// type: Bool;
using boolDataNode = bool;

// type: String;
using stringDataNode = std::string;

// type: Array (Number / String);
using arrayDataNode = std::vector<TypeRoot*>;

// type: Map (Object);
using mapDataNode = std::map<std::string, TypeRoot*>;
using mapKeyDataNode = std::vector<std::string>;
using mapDataNodeSingle = std::pair<std::string, TypeRoot*>;

};  // namespace sharpen_type

#endif  // LIB_CORE_CORE_TYPE_H_
