/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include <type_traits>
#include "lib/core/core_type_class.h"

namespace sharpen_core {

std::ostream& operator<<(std::ostream &out, const TypeRoot &t) {
    return out << t.toJson();
}

// class "Bool";
const std::string Bool::toJson(void) const {
    return this->getNativeData() ? "true" : "false";;
}

// class "String";
const std::string String::toJson(void) const {
    return "\"" + this->getNativeData() + "\"";
}

const bool String::operator==(const std::string &str) const {
    return (this->getNativeData() == str);
}

const bool String::operator==(const char *cs) const {
    return (this->getNativeData() == std::string(cs));
}

// class "Array";
const std::string Array::toJson(void) const {
    std::string s = "[";
    auto n = this->getNativeData();
    for (auto i = begin(n); i != end(n); i++) {
        if (std::next(i) == end(n)) {
            s += ((*i)->toJson());
        } else {
            s += ((*i)->toJson() + ", ");
        }
    }
    return (s + "]");
}

// class "Map";
const std::string Map::toJson(void) const {
    std::string s = "{";
    auto n = this->getNativeData();
    for (auto i = begin(n); i != end(n); i++) {
        if (std::next(i) == end(n)) {
            s += ("\"" + (*i).first + "\": " + (*i).second->toJson());
        } else {
            s += ("\"" + (*i).first + "\": " + (*i).second->toJson() + ", ");
        }
    }
    return (s + "}");
}

mapKeyDataNode Map::getKeyListData(void) {
    // sort first;
    std::sort((this->k).begin(), (this->k).end());
    return this->k;
}

std::shared_ptr<TypeRoot> Map::getValue(const char *key) const {
    auto it = this->n.find(key);
    if (it != this->n.end()) {
        return it->second;
    } else {
        return std::make_shared<Map>();
    }
}


}  // namespace sharpen_core
