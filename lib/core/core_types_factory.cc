#include "core_types_factory.h"

using namespace types;

template<typename T>
T* core::TypeFactory::downCastP (TypeRoot* t) {
    // not safe, but efficient [static_cast];
    return static_cast<T*>(t);
}

bool core::TypeFactory::isEqual (TypeRoot* lv, TypeRoot* rv) {
    JSTypes JSType = lv->getType();

    if (JSType == rv->getType()) {
        switch (JSType) {
            case JSTYPE_INTEGER: {
                return (downCastP<Number<int>>(lv)->getNativeData() == 
                    downCastP<Number<int>>(rv)->getNativeData());
                break;
            }
            case JSTYPE_FLOAT: {
                return (downCastP<Number<double>>(lv)->getNativeData() == 
                    downCastP<Number<double>>(rv)->getNativeData());
                break;
            }
            case JSTYPE_BOOL: {
                return (downCastP<Bool>(lv)->getNativeData() == 
                    downCastP<Bool>(rv)->getNativeData());
                break;
            }
                
            case JSTYPE_STRING: {
                return (downCastP<String>(lv)->getNativeData() == 
                    downCastP<String>(rv)->getNativeData());
                break;
            }
            case JSTYPE_ARRAY: {
                size_t _s;

                auto lvtNative = downCastP<Array>(lv)->getNativeData();
                auto rvtNative = downCastP<Array>(rv)->getNativeData(); 

                if ((_s = lvtNative.size()) == rvtNative.size()) {
                    for (auto i = 0; i < _s; i++) {
                        if (!TypeFactory::isEqual(lvtNative.at(i), rvtNative.at(i)))
                            return false;
                    }
                    return true;
                } else {
                    return false;
                }
                break;
            }
            case JSTYPE_MAP: {
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
                            if (!TypeFactory::isEqual(lvtNative.find(key)->second, _t->second))
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
