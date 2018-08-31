#ifndef __CORE_ERRORS__
#define __CORE_ERRORS__

#include <string>
#include <map>
#include <iostream>

using stringMapType = std::map<std::string, std::string>;

namespace core {
    class Error {
        static stringMapType entity;
    public:
        Error () = default; 
        static void say(std::string key) {
            std::cerr << Error::entity.find(key)->second << std::endl;
        }
    };
}

#endif
