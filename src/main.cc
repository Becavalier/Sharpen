#include "emscripten.h"
#include "../lib/core/core_type_factory.h"
#include "../lib/parser/json.h"
#include "../lib/vdom/vdom.h"
#include <iostream>

// #define DEV 

using namespace sharpen_core;
using namespace sharpen_parser;
using namespace sharpen_vdom;

#ifdef DEV
int main (int argc, char** argv) {

    // ^Number:int;
    Number<int>* numberIntE = TypeFactory::buildNumber(1);
    int numberIntBasic = numberIntE->getNativeData();

    // ^Number:double;
    Number<double>* numberFloatE = TypeFactory::buildNumber(1.5);
    double numberFloatBasic = numberFloatE->getNativeData();

    // ^Bool;
    Bool* boolEA = TypeFactory::buildBool(true);
    Bool* boolEB = TypeFactory::buildBool(false);

    // ^String;
    String* stringE = TypeFactory::buildString("YHSPY");

    // ^Array;
    Array* arrayEA = TypeFactory::buildArray();
    arrayEA->addItem(numberIntE);
    arrayEA->addItem(numberFloatE);
    arrayEA->addItem(stringE);
    arrayEA->addItem(boolEA);

    Array* arrayEB = TypeFactory::buildArray();
    arrayEB->addItem(numberIntE);
    arrayEB->addItem(numberFloatE);
    arrayEB->addItem(stringE);
    arrayEB->addItem(boolEA);

    // ^Map;
    Map* mapEA = TypeFactory::buildMap();
    mapEA->addItem("numberIntE", numberIntE);
    mapEA->addItem("numberFloatE", numberFloatE);
    mapEA->addItem("arrayEA", arrayEA);
    mapEA->addItem("boolEA", boolEA);

    Map* mapEB = TypeFactory::buildMap();
    mapEB->addItem("numberIntE", numberIntE);
    mapEB->addItem("numberFloatE", numberFloatE);
    mapEB->addItem("arrayEA", arrayEA);
    mapEB->addItem("boolEA", boolEA);
    
    // print structure;
    std::cout << *numberIntE << std::endl;
    std::cout << *numberFloatE << std::endl;
    std::cout << *boolEA << std::endl;
    std::cout << *stringE << std::endl;
    std::cout << *arrayEA << std::endl;
    std::cout << arrayEA->getSize() << std::endl;
    std::cout << *mapEA << std::endl;

    // equal?
    std::cout << TypeFactory::isEqual(TypeFactory::buildNumber(1), numberIntE) << std::endl;
    std::cout << TypeFactory::isEqual(TypeFactory::buildBool(true), boolEA) << std::endl;
    std::cout << TypeFactory::isEqual(TypeFactory::buildString("YHSPY"), stringE) << std::endl;
    std::cout << TypeFactory::isEqual(arrayEA, arrayEB) << std::endl;
    std::cout << TypeFactory::isEqual(mapEA, mapEB) << std::endl;

    // "toJson";
    std::cout << arrayEB->toJson() << std::endl;
    std::cout << mapEA->toJson() << std::endl;


    // json parser;
    std::string os = "{          \
        'tagName':'DIV',         \
        'hash':1,                \
        'attributes': {          \
            'id':'native'        \
        },                       \
        'children': {            \
            'tagName':'SPAN',    \
            'hash':2,            \
            'attributes': {},    \
            'innerText':'Apple', \
            'type':'endpoint'    \
        },                       \
        'type':'relay'           \
    }";

    std::string ts = "{          \
        'tagName':'SPAN',        \
        'hash':1,                \
        'attributes': {          \
            'id':'native'        \
        },                       \
        'children': {            \
            'tagName':'SPAN',    \
            'hash':2,            \
            'attributes': {},    \
            'innerText':'Apple', \
            'type':'endpoint'    \
        },                       \
        'type':'relay'           \
    }";

    RSJresource jsonRes(os);

    Map* m = static_cast<Map*>(jsonRes.parseAll());
    std::cout << *m << std::endl;


    // vdom && diff;
    vDOM* o = new vDOM(os);
    vDOM* t = new vDOM(ts);
    std::cout << *(o->to(t)) << std::endl;

    return 0;
}
#endif

extern "C" {
    int EMSCRIPTEN_KEEPALIVE add (int x, int y) {
        return x + y;
    }

    const char* EMSCRIPTEN_KEEPALIVE patch (const char *os, const char *ts) {
        vDOM* o = new vDOM(os);
        vDOM* t = new vDOM(ts);
        return ((o->to(t))->toJson()).c_str();
    }
}
