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

    public:
        Number (const T &data) : TypeRoot(JSTYPE_STRING) {
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

    public:
        String(const std::string &s) : TypeRoot(JSTYPE_STRING), n(s) {}

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
        
    public:
        Array() : TypeRoot(JSTYPE_ARRAY) {}

        inline const std::string getPrintData (void) const override {
            string _t = "[";
            for (auto e : this->n) {
                _t += ("\"" + e->getPrintData() + "\", ");
            }
            return _t + "EOA]";
        }

        inline void addItem (TypeRoot &t) {
            this->n.push_back(&t);
        }

        inline std::vector<JSTypes>::size_type getSize () {
            return this->n.size();
        }
    };


    // "Map" class definition;
    class Map : public TypeRoot {
        friend ostream& operator<<(ostream &out, const TypeRoot &t);

        mapDataNode n;

    public:
        Map() : TypeRoot(JSTYPE_MAP) {}

        inline const std::string getPrintData (void) const override {
            std::string _t = "{";
            for(auto e : this->n) {
                // special case;
                if (e.second->getType() == JSTYPE_ARRAY || 
                    e.second->getType() == JSTYPE_MAP) {
                    _t += ("\"" + e.first + "\": " + e.second->getPrintData() + ", ");
                } else {
                    _t += ("\"" + e.first + "\": \"" + e.second->getPrintData() + "\", ");
                }
            }

            return _t + "EOM}";;
        }

        inline void addItem (std::string key, TypeRoot* value) {
            this->n.insert(std::make_pair(key, value));
        }
    };
};
