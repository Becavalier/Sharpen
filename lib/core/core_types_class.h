#ifndef __CORE_TYPES_CLASS__
#define __CORE_TYPES_CLASS__

#include "core_types.h"
#include "core_errors.h"
#include "core_util.h"

#include <type_traits>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace types;

namespace core {

    // operators override;
    std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

    // "Number" class definition;
    template<typename T>
    class Number : public TypeRoot {    
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

        numberDataNode *n;

    public:
        Number (const T &data) : TypeRoot(Util::getNumberType(data))  {
            this->n = (numberDataNode*) malloc(sizeof(numberDataNode));

            if (std::is_integral<T>::value) {
                this->n->i = data;
            } else if (std::is_floating_point<T>::value) {
                this->n->f = data;
            } else {
                Error::say("INVALID_ARGUMENT");
            }
        }

        ~Number () {
            free(this->n);
        }

        inline const T getNativeData (void) const {
            return this->getType() == JSTYPE_INTEGER ? this->n->i : this->n->f;
        };

        inline const std::string getPrintData (void) const override {
            return std::to_string(this->getNativeData());
        }
    };  


    // "Bool" class definition;
    class Bool : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

        boolDataNode n;
    
    public:
        Bool(const bool &data) : TypeRoot(JSTYPE_BOOL), n(data) {}

        inline const bool getNativeData (void) const {
            return this->n;
        };

        inline const std::string getPrintData (void) const override {
            return this->n ? "true" : "false";
        }
    };

    
    // "String" class definition;
    class String : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

        stringDataNode n; 

    public:
        String(const std::string &data) : TypeRoot(JSTYPE_STRING), n(data) {}

        inline const std::string getNativeData (void) const {
            return this->n;
        };

        inline const std::string getPrintData (void) const override {
            return this->n;
        }
    };

    
    // "Array" class definition;
    class Array : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

        arrayDataNode n;
        
    public:
        Array() : TypeRoot(JSTYPE_ARRAY) {}

        inline const std::string getPrintData (void) const override {
            std::string _t = "[";
            for (auto e : this->n) {
                _t += ("\"" + e->getPrintData() + "\", ");
            }
            return _t + "EOA]";
        }

        inline arrayDataNode getNativeData (void) const {
            return this->n;
        }

        inline void addItem (TypeRoot* t) {
            this->n.push_back(t);
        }

        inline std::vector<JSTypes>::size_type getSize () {
            return this->n.size();
        }
    };


    // "Map" class definition;
    class Map : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);

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

        inline mapDataNode getNativeData (void) const {
            return this->n;
        }

        inline void addItem (std::string key, TypeRoot* value) {
            this->n.insert(std::make_pair(key, value));
        }
    };

};

#endif
