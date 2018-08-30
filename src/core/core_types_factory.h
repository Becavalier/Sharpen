#pragma once

#include "core_types_class.h"

#include <string>

using namespace core;

class TypeFactory {
public:
    TypeFactory () = default;

    // factory methods;
    Number<double> buildNumber (double t) {
        return Number<double>(t);
    }

    Number<int> buildNumber (int t) {
        return Number<int>(t);
    }

    String buildString (const string &s) {
        return String(s);
    }

    Array buildArray () {
        return Array();
    }
};
