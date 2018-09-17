/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_type_factory.h"
#include <string>
#include <iostream>

using sharpen_type::JSTypes;

template<typename T>
T* sharpen_core::TypeFactory::downCastP(TypeRoot* t) {
    // not safe, but efficient [static_cast];
    return static_cast<T*>(t);
}

bool sharpen_core::TypeFactory::isEqual(TypeRoot* lv, TypeRoot* rv) {
    JSTypes JSType = lv->getType();

    if (JSType == rv->getType()) {
        switch (JSType) {
            case JSTypes::JSTYPE_INTEGER: {
                return (downCastP<Number<int>>(lv)->getNativeData() ==
                    downCastP<Number<int>>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_FLOAT: {
                return (downCastP<Number<double>>(lv)->getNativeData() ==
                    downCastP<Number<double>>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_BOOL: {
                return (downCastP<Bool>(lv)->getNativeData() ==
                    downCastP<Bool>(rv)->getNativeData());
                break;
            }

            case JSTypes::JSTYPE_STRING: {
                return (downCastP<String>(lv)->getNativeData() ==
                    downCastP<String>(rv)->getNativeData());
                break;
            }
            case JSTypes::JSTYPE_ARRAY: {
                size_t _s;

                auto lvtNative = downCastP<Array>(lv)->getNativeData();
                auto rvtNative = downCastP<Array>(rv)->getNativeData();

                if ((_s = lvtNative.size()) == rvtNative.size()) {
                    for (auto i = 0; i < _s; i++) {
                        if (!TypeFactory::isEqual(
                            lvtNative.at(i),
                            rvtNative.at(i)))
                            return false;
                    }
                    return true;
                } else {
                    return false;
                }
                break;
            }
            case JSTypes::JSTYPE_MAP: {
                size_t _s;

                auto lvtNative = downCastP<Map>(lv)->getNativeData();
                auto rvtNative = downCastP<Map>(rv)->getNativeData();

                // get keys;
                auto lvtKeyList = downCastP<Map>(lv)->getKeyListData();

                if ((_s = lvtNative.size()) == rvtNative.size()) {
                    for (auto i = 0; i < _s; i++) {
                        std::string key = lvtKeyList.at(i);
                        // compare;
                        auto _t = rvtNative.find(key);
                        if (_t != end(rvtNative)) {
                            if (!TypeFactory::isEqual(
                                lvtNative.find(key)->second,
                                _t->second))
                                return false;
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

std::string sharpen_core::TypeFactory::splitStr(
    std::string str,
    char dir,
    char del) {
    std::size_t strIndex = str.find(del);

    if (dir == 'l') {
        return str.substr(0, strIndex);
    } else if (dir == 'r') {
        return str.substr(strIndex + 1, std::string::npos);
    }

    return str;
}

std::string sharpen_core::TypeFactory::replaceStr(
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

bool sharpen_core::TypeFactory::splitEqual(
    String *lstr,
    String *rstr,
    char dir,
    char del
) {
    std::string lstrVal = lstr->getNativeData();
    std::string rstrVal = rstr->getNativeData();

    return splitEqual(lstrVal, rstrVal, dir, del);
}

bool sharpen_core::TypeFactory::splitEqual(
    std::string lstr,
    std::string rstr,
    char dir,
    char del
) {
    return (splitStr(lstr, dir, del) == splitStr(rstr, dir, del));
}
