/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_TYPE_FACTORY_H_
#define LIB_CORE_CORE_TYPE_FACTORY_H_

#include <string>
#include <memory>
#include "lib/core/core_type_class.h"
#include "lib/core/core_util.h"

using sharpen_type::TypeRoot;

namespace sharpen_core {

class TypeFactory {
 public:
    TypeFactory() = default;
    ~TypeFactory() = default;
    static bool isEqual(std::shared_ptr<TypeRoot>, std::shared_ptr<TypeRoot>);

    // factory methods;
    static inline std::shared_ptr<Number<double>> buildNumber(double t) {
        return std::make_shared<Number<double>>(t);
    }

    static inline std::shared_ptr<Number<int>> buildNumber(int t) {
        return std::make_shared<Number<int>>(t);
    }

    static inline std::shared_ptr<Bool> buildBool(bool t) {
        return std::make_shared<Bool>(t);
    }

    static inline std::shared_ptr<String> buildString(std::string s) {
        return std::make_shared<String>(s);
    }

    static inline std::shared_ptr<Array> buildArray() {
        return std::make_shared<Array>();
    }

    static inline std::shared_ptr<Map> buildMap() {
        return std::make_shared<Map>();
    }
};

}  // namespace sharpen_core

#endif  // LIB_CORE_CORE_TYPE_FACTORY_H_
