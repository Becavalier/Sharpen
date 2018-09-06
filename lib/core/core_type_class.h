#ifndef __CORE_TYPES_CLASS__
#define __CORE_TYPES_CLASS__

#include "core_type.h"
#include "core_error.h"
#include "core_util.h"

#include <type_traits>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace sharpen_type;

namespace sharpen_core {

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

        const std::string toJson(void) const override {
            return std::to_string(this->getNativeData());
        };

        const T getNativeData (void) const {
            return this->getType() == JSTYPE_INTEGER ? this->n->i : this->n->f;
        }
    };  


    // "Bool" class definition;
    class Bool : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
        boolDataNode n;
    
    public:
        Bool(const bool &data) : TypeRoot(JSTYPE_BOOL), n(data) {}
        ~Bool() = default;
        const std::string toJson(void) const override;

        const bool getNativeData (void) const {
            return this->n;
        }
    };

    
    // "String" class definition;
    class String : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
        stringDataNode n; 

    public:
        String(const std::string &data) : TypeRoot(JSTYPE_STRING), n(data) {}
        String(const char *data) : String(std::string(data)) {}
        ~String() = default;
        const std::string toJson(void) const override;

        const std::string getNativeData (void) const {
            return this->n;
        }
    };

    
    // "Array" class definition;
    class Array : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
        arrayDataNode n;
        
    public:
        Array() : TypeRoot(JSTYPE_ARRAY) {}
        ~Array() = default;
        const std::string toJson(void) const override;

        arrayDataNode getNativeData (void) const {
            return this->n;
        }

        void addItem (TypeRoot* t) {
            this->n.push_back(t);
        }

        std::vector<JSTypes>::size_type getSize () {
            return this->n.size();
        }
    };


    // "Map" class definition;
    class Map : public TypeRoot {
        friend std::ostream& operator<<(std::ostream &out, const TypeRoot &t);
        mapDataNode n;
        mapKeyDataNode k;

    public:
        Map() : TypeRoot(JSTYPE_MAP) {}
        ~Map() = default;
        const std::string toJson(void) const override;

        mapDataNode getNativeData (void) const {
            return this->n;
        }

        mapKeyDataNode getKeyListData (void) const {
            return this->k;
        }

        TypeRoot* getValue (const char *key) const {
            return this->n.find(key)->second;
        }

        void addItem (std::string key, TypeRoot* value) {
            this->k.push_back(key);
            this->n.insert(std::make_pair(key, value));
        }
    };

};

#endif
