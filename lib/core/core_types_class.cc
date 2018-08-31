#include "core_types_class.h"

namespace core {
    std::ostream& operator<<(std::ostream &out, const TypeRoot &t) {
        return out << t.getPrintData();
    }
}
