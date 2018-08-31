#include "../lib/core/core_types_factory.h"
#include <iostream>

using namespace core;

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
    Map* mapE = factory->buildMap();
    mapE->addItem("numberIntE", numberIntE);
    mapE->addItem("numberFloatE", numberFloatE);
    mapE->addItem("arrayE", arrayEA);
    mapE->addItem("boolEA", boolEA);

    
    // print structure;
    std::cout << *numberIntE << std::endl;
    std::cout << *numberFloatE << std::endl;
    std::cout << *boolEA << std::endl;
    std::cout << *stringE << std::endl;
    std::cout << *arrayEA << std::endl;
    std::cout << arrayEA->getSize() << std::endl;
    std::cout << *mapE << std::endl;

    // equal?
    std::cout << TypeFactory::isEqual(factory->buildNumber(1), numberIntE) << std::endl;
    std::cout << TypeFactory::isEqual(factory->buildBool(true), boolEA) << std::endl;
    std::cout << TypeFactory::isEqual(factory->buildString("YHSPY"), stringE) << std::endl;
    std::cout << TypeFactory::isEqual(arrayEA, arrayEB) << std::endl;

    return 0;
}