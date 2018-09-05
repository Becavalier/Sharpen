#include "core_type_class.h"

namespace sharpen_core {
    std::ostream& operator<<(std::ostream &out, const TypeRoot &t) {
        return out << t.getPrintData();
    }
}
