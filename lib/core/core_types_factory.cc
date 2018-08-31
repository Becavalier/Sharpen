#include "core_types_factory.h"

using namespace types;

bool core::TypeFactory::isEqual (TypeRoot* lv, TypeRoot* rv) {
    JSTypes JSType = lv->getType();

    // not safe, but efficient [static_cast];
    if (JSType == rv->getType()) {
        switch (JSType) {
            case JSTYPE_INTEGER: {
                return (static_cast<Number<int>*>(lv)->getNativeData() == 
                    static_cast<Number<int>*>(rv)->getNativeData());
                break;
            }
            case JSTYPE_FLOAT: {
                return (static_cast<Number<double>*>(lv)->getNativeData() == 
                    static_cast<Number<double>*>(rv)->getNativeData());
                break;
            }
            case JSTYPE_BOOL: {
                return (static_cast<Bool*>(lv)->getNativeData() == 
                    static_cast<Bool*>(rv)->getNativeData());
                break;
            }
                
            case JSTYPE_STRING: {
                return (static_cast<String*>(lv)->getNativeData() == 
                    static_cast<String*>(rv)->getNativeData());
                break;
            }
            case JSTYPE_ARRAY: {
                size_t _s;

                auto lvtNative = static_cast<Array*>(lv)->getNativeData();
                auto rvtNative = static_cast<Array*>(rv)->getNativeData(); 

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
