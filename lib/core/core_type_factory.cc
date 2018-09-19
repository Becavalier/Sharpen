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

std::string TypeFactory::splitStr(
    std::string str,
    char dir,
    char del
) {
    std::size_t strIndex = str.find(del);

    if (dir == 'l') {
        return str.substr(0, strIndex);
    } else if (dir == 'r') {
        return str.substr(strIndex + 1, std::string::npos);
    }

    return str;
}

std::string TypeFactory::replaceStr(
    std::string str,
    std::string replacement,
    char dir,
    char del
) {
    std::size_t strIndex = str.find(del);

    if (dir == 'l') {
        return str.replace(0, strIndex, replacement);
    } else if (dir == 'r') {
        return str.replace(strIndex + 1, std::string::npos, replacement);
    }

    return str;
}

bool TypeFactory::splitEqual(
    std::shared_ptr<String> lstr,
    std::shared_ptr<String> rstr,
    char dir,
    char del
) {
    std::string lstrVal = lstr->getNativeData();
    std::string rstrVal = rstr->getNativeData();

    return splitEqual(lstrVal, rstrVal, dir, del);
}

bool TypeFactory::splitEqual(
    std::string lstr,
    std::string rstr,
    char dir,
    char del
) {
    return (splitStr(lstr, dir, del) == splitStr(rstr, dir, del));
}

}  // namespace sharpen_core
