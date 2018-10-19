/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_TYPE_H_
#define LIB_CORE_CORE_TYPE_H_

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

namespace sharpen_type {

enum class JSTypes : char {
    JSTYPE_INTEGER = 0,
    JSTYPE_FLOAT,
    JSTYPE_STRING,
    JSTYPE_BOOL,
    JSTYPE_ARRAY,
    JSTYPE_MAP
};

// base type class;
class TypeRoot : public std::enable_shared_from_this<TypeRoot> {
    JSTypes type;
 public:
    explicit TypeRoot(JSTypes JSType) : type(JSType) {}
    ~TypeRoot() = default;
    // interfaces (pure virtual);
    virtual const std::string toJson(void) const = 0;

    const JSTypes getType(void) const {
        return this->type;
    }

    template<typename T>
    std::shared_ptr<T> as() {
        // [shared reference] + 1;
        return std::static_pointer_cast<T>(shared_from_this());
    }
};

using numberDataNode = union {
    int i;
    double f;
};

// types for class "Bool";
using boolDataNode = bool;

// types for class "String";
using stringDataNode = std::string;

// types for class "Array";
using arrayDataNode = std::vector<std::shared_ptr<TypeRoot>>;

// types for class "Map";
using mapDataNode = std::map<std::string, std::shared_ptr<TypeRoot>>;
using mapKeyDataNode = std::vector<std::string>;

};  // namespace sharpen_type

#endif  // LIB_CORE_CORE_TYPE_H_
