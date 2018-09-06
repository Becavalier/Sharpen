#ifndef __PARSER_JSON__
#define __PARSER_JSON__

#include "../core/core_type_factory.h"

#include <string>
#include <vector>
#include <utility>

using namespace sharpen_core;
using namespace sharpen_type;

namespace sharpen_parser {

    std::string strtrim (std::string str, std::string chars = " \t\n\r", std::string opts = "lr");
    std::string strip_outer_quotes (std::string str, char* qq = nullptr);
    int is_bracket (char c, std::vector<char const*>& bracks, int indx = 0);

    // [boottleneck];
    std::vector<std::string> split_RSJ_array (std::string str);
    std::string insert_tab_after_newlines (std::string str);

    class RSJresource {
        std::string data; 
        TypeRoot* n;
        
    public:
        RSJresource (std::string str) : data (str) {}
        RSJresource (const char* str) : RSJresource(std::string(str)) {}

        ~RSJresource() {};
        
        TypeRoot* parseAll (void); 
        TypeRoot* parse (const std::string& data);
    };

}

#endif
