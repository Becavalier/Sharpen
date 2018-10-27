/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include <iostream>
#include "lib/core/core_type_factory.h"

using sharpen_type::JSTypes;

namespace sharpen_core {

bool TypeFactory::isEqual(std::shared_ptr<TypeRoot> lv, std::shared_ptr<TypeRoot> rv) {
    JSTypes type = lv->getType();

    if (type == rv->getType()) {
        switch (type) {
            case JSTypes::JSTYPE_INTEGER: {
                return (Util::DCP<Number<int>>(lv)->getNativeData() ==
                    Util::DCP<Number<int>>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_FLOAT: {
                return (Util::DCP<Number<double>>(lv)->getNativeData() ==
                    Util::DCP<Number<double>>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_BOOL: {
                return (Util::DCP<Bool>(lv)->getNativeData() ==
                    Util::DCP<Bool>(rv)->getNativeData());
                break;
            }

            case JSTypes::JSTYPE_STRING: {
                return (Util::DCP<String>(lv)->getNativeData() ==
                    Util::DCP<String>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_ARRAY: {
                auto lvNative = Util::DCP<Array>(lv)->getNativeData();
                auto rvNative = Util::DCP<Array>(rv)->getNativeData();

                size_t s;
                if ((s = lvNative.size()) == rvNative.size()) {
                    for (auto i = 0; i < s; i++) {
                        if (!TypeFactory::isEqual(lvNative.at(i), rvNative.at(i))) {
                            return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
                break;
            }
            case JSTypes::JSTYPE_MAP: {
                auto lvNative = Util::DCP<Map>(lv)->getNativeData();
                auto rvNative = Util::DCP<Map>(rv)->getNativeData();

                // get keys;
                auto lvKeyList = Util::DCP<Map>(lv)->getKeyListData();

                size_t s;
                if ((s = lvNative.size()) == rvNative.size()) {
                    for (auto i = 0; i < s; i++) {
                        std::string key = lvKeyList.at(i);
                        // compare;
                        auto v = rvNative.find(key);
                        if (v != end(rvNative)) {
                            if (!TypeFactory::isEqual(lvNative.find(key)->second, v->second)) {
                                return false;
                            }
                        } else {
                            return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
                break;
            }
            default: {
                return false;
                break;
            }
        }
    }
    return false;
}

}  // namespace sharpen_core
