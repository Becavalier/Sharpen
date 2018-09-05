#include "../lib/core/core_type_factory.h"
#include "../lib/parser/json.h"
#include <iostream>

using namespace sharpen_core;
using namespace sharpen_parser;

int main (int argc, char** argv) {
    TypeFactory* factory = new TypeFactory();
    
    // ^Number:int;
    Number<int>* numberIntE = factory->buildNumber(1);
    int numberIntBasic = numberIntE->getNativeData();

    // ^Number:double;
    Number<double>* numberFloatE = factory->buildNumber(1.5);
    double numberFloatBasic = numberFloatE->getNativeData();

    // ^Bool;
    Bool* boolEA = factory->buildBool(true);
    Bool* boolEB = factory->buildBool(false);

    // ^String;
    String* stringE = factory->buildString("YHSPY");

    // ^Array;
    Array* arrayEA = factory->buildArray();
    arrayEA->addItem(numberIntE);
    arrayEA->addItem(numberFloatE);
    arrayEA->addItem(stringE);
    arrayEA->addItem(boolEA);

    Array* arrayEB = factory->buildArray();
    arrayEB->addItem(numberIntE);
    arrayEB->addItem(numberFloatE);
    arrayEB->addItem(stringE);
    arrayEB->addItem(boolEA);

    // ^Map;
    Map* mapEA = factory->buildMap();
    mapEA->addItem("numberIntE", numberIntE);
    mapEA->addItem("numberFloatE", numberFloatE);
    mapEA->addItem("arrayEA", arrayEA);
    mapEA->addItem("boolEA", boolEA);

    Map* mapEB = factory->buildMap();
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
    std::cout << TypeFactory::isEqual(factory->buildNumber(1), numberIntE) << std::endl;
    std::cout << TypeFactory::isEqual(factory->buildBool(true), boolEA) << std::endl;
    std::cout << TypeFactory::isEqual(factory->buildString("YHSPY"), stringE) << std::endl;
    std::cout << TypeFactory::isEqual(arrayEA, arrayEB) << std::endl;
    std::cout << TypeFactory::isEqual(mapEA, mapEB) << std::endl;


    // json;
    std::string str = "{'animal':cat, coordinates: [2, 5, 8], height: 1, \nis_vicious: false, comment:'It\\'s in fact quite...\\t adorable.' }";

    // init parser;
    RSJresource jsonRes(str);

    // parsing;
    Map* m = static_cast<Map*>(jsonRes.parseAll());
    
    // print result;
    std::cout << *m << std::endl;

    return 0;
}