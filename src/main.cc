#include "core/core_types_factory.h"
#include <iostream>

using namespace core;

int main (int argc, char** argv) {
    TypeFactory* factory = new TypeFactory();
    
    // ^Number:int;
    Number<int> numberIntE = factory->buildNumber(1);
    int numberIntBasic = numberIntE.getNativeData();

    // ^Number:double;
    Number<double> numberFloatE = factory->buildNumber(1.5);
    double numberFloatBasic = numberFloatE.getNativeData();

    // ^String;
    String stringE = factory->buildString("This is a string.");

    // ^Array;
    Array arrayE = factory->buildArray();
    arrayE.addItem(numberIntE);
    arrayE.addItem(numberFloatE);
    arrayE.addItem(stringE);

    // ^Map;
    Map mapE = factory->buildMap();
    mapE.addItem("numberIntE", &numberIntE);
    mapE.addItem("numberFloatE", &numberFloatE);
    mapE.addItem("arrayE", &arrayE);

    
    // print structure;
    std::cout << numberIntE << std::endl;
    std::cout << numberFloatE << std::endl;
    std::cout << stringE << std::endl;
    std::cout << arrayE << std::endl;
    std::cout << arrayE.getSize() << std::endl;
    std::cout << mapE << std::endl;

    return 0;
}