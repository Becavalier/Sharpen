#include "core_type_class.h"
#include <type_traits>

namespace sharpen_core {

    std::ostream& operator<<(std::ostream &out, const TypeRoot &t) {
        return out << t.toJson();
    }

    // class Bool;
    const std::string Bool::toJson (void) const {
        return this->getNativeData() ? "true" : "false";;
    }

    // class String;
    const std::string String::toJson (void) const {
        return "\"" + this->getNativeData() + "\""; 
    }

    // class Array;
    const std::string Array::toJson (void) const {
        std::string _t = "[";
        auto _n = this->getNativeData();
        for (auto i = begin(_n); i != end(_n); i++) {
            if (std::next(i) == end(_n))
                _t += ((*i)->toJson());
            else
                _t += ((*i)->toJson() + ", ");
        }
        return (_t + "]");
    };

    // class Map;
    const std::string Map::toJson (void) const {
        std::string _t = "{";
        auto _n = this->getNativeData();
        for (auto i = begin(_n); i != end(_n); i++) {
            if (std::next(i) == end(_n))
                _t += ("\"" + (*i).first + "\": " + (*i).second->toJson());
            else
                _t += ("\"" + (*i).first + "\": " + (*i).second->toJson() + ", ");
        }
        return (_t + "}");
    }

}
