/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_TYPE_CLASS_H_
#define LIB_CORE_CORE_TYPE_CLASS_H_

#include <type_traits>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "lib/core/core_type.h"
#include "lib/core/core_error.h"
#include "lib/core/core_util.h"

using sharpen_type::JSTypes;
using sharpen_type::TypeRoot;
using sharpen_type::numberDataNode;
using sharpen_type::stringDataNode;
using sharpen_type::boolDataNode;
using sharpen_type::arrayDataNode;
using sharpen_type::mapDataNode;
using sharpen_type::mapKeyDataNode;

namespace sharpen_core {

// operators override;
std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

// class "Number" definition;
template<typename T>
class Number : public TypeRoot {
    friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
    std::shared_ptr<numberDataNode> n;

 public:
    explicit Number(const T &data) : TypeRoot(Util::getNumberType(data))  {
        this->n = std::shared_ptr<numberDataNode>(
            static_cast<numberDataNode*>(malloc(sizeof(numberDataNode))));

        if (std::is_integral<T>::value) {
            this->n->i = data;
        } else if (std::is_floating_point<T>::value) {
            this->n->f = data;
        } else {
            Error::say("INVALID_ARGUMENT");
        }
    }
    ~Number() {}

    const std::string toJson(void) const override {
        return std::to_string(this->getNativeData());
    };

    const T getNativeData(void) const {
        return this->getType() == JSTypes::JSTYPE_INTEGER ? this->n->i : this->n->f;
    }
};


// class "Bool" definition;
class Bool : public TypeRoot {
    friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
    boolDataNode n;

 public:
    explicit Bool(const bool &data) : TypeRoot(JSTypes::JSTYPE_BOOL), n(data) {}
    ~Bool() = default;
    const std::string toJson(void) const override;

    const bool getNativeData(void) const {
        return this->n;
    }
};


// class "String" definition;
class String : public TypeRoot {
    friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
    stringDataNode n;

 public:
    explicit String(const std::string &data) :
        TypeRoot(JSTypes::JSTYPE_STRING),
        n(data) {}
    explicit String(const char *data) : String(std::string(data)) {}
    ~String() = default;
    const bool operator==(const std::string&) const;
    const bool operator==(const char*) const;
    const std::string toJson(void) const override;

    const std::string getNativeData(void) const {
        return this->n;
    }
};


// class "Array" definition;
class Array : public TypeRoot {
    friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
    arrayDataNode n;

 public:
    Array() : TypeRoot(JSTypes::JSTYPE_ARRAY) {}
    ~Array() = default;
    const std::string toJson(void) const override;

    arrayDataNode getNativeData(void) const {
        return this->n;
    }

    void addItem(std::shared_ptr<TypeRoot> t) {
        this->n.push_back(t);
    }

    std::vector<TypeRoot*>::size_type getSize() const {
        return this->n.size();
    }
};


// class "Map" definition;
class Map : public TypeRoot {
    friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
    mapDataNode n;
    mapKeyDataNode k;

 public:
    Map() : TypeRoot(JSTypes::JSTYPE_MAP) {}
    ~Map() = default;
    const std::string toJson(void) const override;
    mapKeyDataNode getKeyListData(void);
    std::shared_ptr<TypeRoot> getValue(const char *key) const;

    mapDataNode getNativeData(void) const {
        return this->n;
    }

    std::shared_ptr<TypeRoot> getValue(const std::string &key) const {
        return this->getValue(key.c_str());
    }

    std::vector<TypeRoot*>::size_type getSize() const {
        return this->k.size();
    }

    template<typename T>
    void addItem(const T &key, std::shared_ptr<TypeRoot> value) {
        std::string _key = Util::toStr(key);

        this->k.push_back(_key);
        this->n.insert(std::make_pair(_key, value));
    }
};

};  // namespace sharpen_core

#endif  // LIB_CORE_CORE_TYPE_CLASS_H_
