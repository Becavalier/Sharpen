#pragma once

#include "core_types.h"
#include "core_errors.h"

#include <type_traits>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace types;

namespace core {
    ostream& operator<<(ostream &out, const TypeRoot &t){
        return out << t.getPrintData();
    }

    // "Number" class definition;
    template<typename T>
    class Number : public TypeRoot {    
        friend ostream& operator<<(ostream &out, const TypeRoot &t);

        numberDataNode *n;
        short symbol;
        JSTypes JSType;

    public:
        Number (const T &data) : TypeRoot(), JSType(JSTYPE_NUMBER) {
            this->n = (numberDataNode*) malloc(sizeof(numberDataNode));

            if (std::is_integral<T>::value) {
                this->n->i = data;
                this->symbol = INTEGER;
            } else if (std::is_floating_point<T>::value) {
                this->n->f = data;
                this->symbol = FLOAT;
            } else {
                std::cerr << errors::INVALID_ARGUMENT << std::endl;
            }
        }

        ~Number () {
            free(this->n);
        }

        inline const T getNativeData (void) const {
            return this->symbol == INTEGER ? this->n->i : this->n->f;
        };

        inline const std::string getPrintData (void) const override {
            return to_string(this->getNativeData());
        }
    };  

    
    // "String" class definition;
    class String : public TypeRoot {
        friend ostream& operator<<(ostream &out, const TypeRoot &t);

        stringDataNode n; 
        JSTypes JSType;

    public:
        String(const std::string &s) : TypeRoot(), JSType(JSTYPE_STRING), n(s) {}

        inline const std::string getNativeData (void) const {
            return this->n;
        };

        inline const std::string getPrintData (void) const override {
            return this->n;
        }
    };

    
    // "Array" class definition;
    class Array : public TypeRoot {
        friend ostream& operator<<(ostream &out, const TypeRoot &t);

        arrayDataNode n = {};
        std::vector<JSTypes> arrayDataNodeTypeList;

        JSTypes JSType;
        
    public:
        Array() : TypeRoot(), JSType(JSTYPE_ARRAY) {}

        inline const std::string getPrintData (void) const override {
            string t = "[";
            for(auto e : this->n) {
                t += ("\"" + e->getPrintData() + "\", ");
            }
            return t + "EOA]";
        }

        inline void addItem (TypeRoot &t) {
            this->n.push_back(&t);
        }

        inline std::vector<JSTypes>::size_type getSize () {
            return this->n.size();
        }
    };

};
