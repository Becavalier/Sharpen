#include "json.h"

namespace sharpen_parser {

    char const* RSJobjectbrackets = "{}";
    char const* RSJarraybrackets = "[]";
    char RSJobjectassignment = ':';
    char RSJarraydelimiter = ',';

    std::vector<char const*> RSJbrackets = {
        RSJobjectbrackets, 
        RSJarraybrackets
    };
    std::vector<char const*> RSJstringquotes = {"\"\"", "''"};
    char RSJcharescape = '\\';
    std::string RSJlinecommentstart = "//";
    std::string RSJprinttab = "    ";
    

    std::string strtrim (std::string str, std::string chars, std::string opts) {
        if (str.empty()) return(str);
        
        if (opts.find('l') != std::string::npos) { 
            int p;
            for (p = 0; p < str.length(); ++p)
                if (chars.find(str[p]) == std::string::npos) break;
            str.erase(0, p);
        }
        
        if (opts.find('r') != std::string::npos) { 
            int q, strlenm1 = str.length()-1;
            for (q = 0; q < str.length(); ++q)
                if (chars.find(str[strlenm1-q]) == std::string::npos) break;
            str.erase(str.length() - q, q);
        }
        
        return (str);
    }

    std::string strip_outer_quotes (std::string str, char* qq) {
        str = strtrim(str);
        
        std::string ret = strtrim(str, "\"");
        if (ret == str) {
            ret = strtrim(str, "'");
            if (qq && ret != str) *qq = '\'';
        }
        else if (qq)
            *qq = '"';
        
        return (ret);
    }


    int is_bracket (char c, std::vector<char const*>& bracks, int indx) {
        for (int b = 0; b < bracks.size(); ++b)
            if (c == bracks[b][indx]) 
                return (b);
        return (-1);
    }

    // [boottleneck];
    std::vector<std::string> split_RSJ_array (std::string str) {
        std::vector<std::string> ret;
        std::string current;
        std::vector<int> bracket_stack;
        std::vector<int> quote_stack;
        bool escape_active = false;
        int bi;
        
        for (int a = 0; a < str.length(); ++a) { 
            
            // delimiter
            if ( bracket_stack.size() == 0  &&  quote_stack.size() == 0  &&  str[a] == RSJarraydelimiter ) {
                ret.push_back (current);
                current.clear(); bracket_stack.clear(); quote_stack.clear(); escape_active = false;
                continue; // to *
            }
            
            // checks for string
            if (quote_stack.size() > 0) { 
                if (str[a] == RSJcharescape)  
                    escape_active = !escape_active;
                else if (!escape_active  &&  str[a] == RSJstringquotes[quote_stack.back()][1] ) { 
                    quote_stack.pop_back();
                    escape_active = false;
                }
                else
                    escape_active = false;
                
                current.push_back(str[a]);
                continue; 
            }
            
            if (quote_stack.size() == 0) { 
                if ((bi = is_bracket(str[a], RSJstringquotes)) >= 0) {
                    quote_stack.push_back(bi);
                    current.push_back(str[a]);
                    continue;
                }
            }
            
            // checks for comments
            if (quote_stack.size() == 0) { 
                
                if (str.compare(a, RSJlinecommentstart.length(), RSJlinecommentstart) == 0) {
                    int newline_pos = str.find("\n", a);
                    if (newline_pos == std::string::npos)
                        newline_pos = str.find("\r", a);
                    
                    if (newline_pos != std::string::npos)
                        a = newline_pos; 
                    else 
                        a = str.length();
                    continue;
                }
            }
            
            // checks for brackets
            if ( bracket_stack.size() > 0  &&  str[a] == RSJbrackets[bracket_stack.back()][1] ) { 
                bracket_stack.pop_back();
                current.push_back(str[a]);
                continue;
            }
            
            if ((bi = is_bracket(str[a], RSJbrackets)) >= 0) {
                bracket_stack.push_back (bi);
                current.push_back(str[a]);
                continue; 
            }
            
            // otherwise
            current.push_back(str[a]);
        }
        
        if (current.length() > 0)
            ret.push_back(current);
        
        return (ret);
    }

    std::string insert_tab_after_newlines (std::string str) {
        for (int a = 0; a < str.length(); ++a)
            if (str[a] == '\n') {
                str.insert(a+1, RSJprinttab);
                a += RSJprinttab.length();
            }
        return (str);
    }

    TypeRoot* RSJresource::parse (const std::string& data) {
        TypeRoot* _t;
        std::string content = strtrim(data);  
        
        // parse as object;
        content = strtrim(strtrim(content, " {", "l" ), " }", "r");
        if (content.length() != data.length()) {
            _t = this->factory->buildMap();
            std::vector<std::string> nvPairs = split_RSJ_array(content);
            for (int a = 0; a < nvPairs.size(); ++a) {
                std::size_t assignmentPos = nvPairs[a].find(RSJobjectassignment);
                static_cast<Map*>(_t)->addItem(
                    strip_outer_quotes(nvPairs[a].substr(0, assignmentPos)), 
                    this->parse(strtrim(nvPairs[a].substr(assignmentPos + 1))));
            }
            return _t;
        }
        
        // parse as array;
        content = strtrim(strtrim(content, " [", "l" ), " ]", "r" );
        if (content.length() != data.length()) {
            _t = this->factory->buildArray();
            std::vector<std::string> nvPairs = split_RSJ_array(content);
            for (int a = 0; a < nvPairs.size(); ++a) {
                static_cast<Array*>(_t)->addItem(this->parse(strtrim(nvPairs[a])));
            }
            return _t;
        }

        // parse as string;
        return this->factory->buildString(data);
    }

    TypeRoot* RSJresource::parseAll (void) {
        return this->parse(this->data);
    }
}
