#include <iostream>
#include <vector>
#include <memory>
#include "lib/core/core_type_factory.h"
#include "lib/core/core_util.h"
#include "lib/parser/json.h"
#include "lib/vdom/vdom.h"

#ifdef WASM
#include "emscripten.h"
#endif

using namespace sharpen_core;
using namespace sharpen_parser;
using namespace sharpen_vdom;

int main (int argc, char** argv) {

    // test ^Number:int;
    std::shared_ptr<Number<int>> numberIntE = TypeFactory::buildNumber(1);
    int numberIntBasic = numberIntE->getNativeData();

    // test ^Number:double;
    std::shared_ptr<Number<double>> numberFloatE = TypeFactory::buildNumber(1.5);
    double numberFloatBasic = numberFloatE->getNativeData();

    // test ^Bool;
    std::shared_ptr<Bool> boolEA = TypeFactory::buildBool(true);
    std::shared_ptr<Bool> boolEB = TypeFactory::buildBool(false);

    // test ^String;
    std::shared_ptr<String> stringE = TypeFactory::buildString("YHSPY");

    // test ^Array;
    std::shared_ptr<Array> arrayEA = TypeFactory::buildArray();
    arrayEA->addItem(numberIntE);
    arrayEA->addItem(numberFloatE);
    arrayEA->addItem(stringE);
    arrayEA->addItem(boolEA);

    std::shared_ptr<Array> arrayEB = TypeFactory::buildArray();
    arrayEB->addItem(numberIntE);
    arrayEB->addItem(numberFloatE);
    arrayEB->addItem(stringE);
    arrayEB->addItem(boolEA);

    // test ^Map;
    std::shared_ptr<Map> mapEA = TypeFactory::buildMap();
    mapEA->addItem("numberIntE", numberIntE);
    mapEA->addItem("numberFloatE", numberFloatE);
    mapEA->addItem("arrayEA", arrayEA);
    mapEA->addItem("boolEA", boolEA);

    std::shared_ptr<Map> mapEB = TypeFactory::buildMap();
    mapEB->addItem("numberIntE", numberIntE);
    mapEB->addItem("numberFloatE", numberFloatE);
    mapEB->addItem("arrayEA", arrayEA);
    mapEB->addItem("boolEA", boolEA);
    

    // test "isEqual";
    auto isEqualNumber = TypeFactory::isEqual(TypeFactory::buildNumber(1), numberIntE);
    auto isEqualBool = TypeFactory::isEqual(TypeFactory::buildBool(true), boolEA);
    auto isEqualString = TypeFactory::isEqual(TypeFactory::buildString("YHSPY"), stringE);
    auto isEqualArray = TypeFactory::isEqual(arrayEA, arrayEB);
    auto isEqualMap = TypeFactory::isEqual(mapEA, mapEB);


    // test "toJson";
    auto arrayJson = arrayEB->toJson();
    auto mapJson = mapEA->toJson();


    // test Json parser;
    std::string os = "{\
        \"16\": \"DIV\",\
        \"hash\": \"o_0\",\
        \"attributes\": {\
            \"id\": \"native\",\
            \"data-tid\": \"apple\",\
            \"data-type\": \"div\"\
        },\
        \"children\": {\
            \"o_1\": {\
                \"16\": \"SPAN\",\
                \"hash\": \"o_1\",\
                \"attributes\": {\
                    \"style\": \"font-size: 12px; font-weight: bold;\"\
                },\
                \"innerText\": \"Apple\",\
                \"type\": 4\
            },\
            \"o_2\": {\
                \"16\": \"H4\",\
                \"hash\": \"o_2\",\
                \"attributes\": {},\
                \"innerText\": \"A very cool company!\",\
                \"type\": 4\
            }\
        },\
        \"type\": 2\
    }";

    std::string ts = "{\
        \"16\": \"DIV\",\
        \"hash\": \"t_0\",\
        \"attributes\": {\
            \"id\": \"patch\",\
            \"onclick\": \"alert(true);\",\
            \"style\": \"background-color: yellow;\",\
            \"data-sid\": \"google\",\
            \"data-type\": \"div\"\
        },\
        \"children\": {\
            \"t_1\": {\
                \"16\": \"SPAN\",\
                \"hash\": \"t_1\",\
                \"attributes\": {\
                    \"style\": \"font-size: 20px; color: blue; font-weight: bold;\"\
                },\
                \"innerText\": \"Google\",\
                \"type\": 4\
            },\
            \"t_2\": {\
                \"16\": \"P\",\
                \"hash\": \"t_2\",\
                \"attributes\": {},\
                \"innerText\": \"An amazing company!\",\
                \"type\": 4\
            },\
            \"t_3\": {\
                \"16\": \"P\",\
                \"hash\": \"t_3\",\
                \"attributes\": {\
                    \"style\": \"color: red;\"\
                },\
                \"innerText\": \"share: $1164.83\",\
                \"type\": 4\
            }\
        },\
        \"type\": 2\
    }";

    // test vdom && diff;
    std::shared_ptr<vDOM> o(new vDOM(os, 'o'));
    std::shared_ptr<vDOM> t(new vDOM(ts, 't'));
    auto diff = o->to(t)->toJson();

    // end;
    std::cout << "[Sharpen] initialized!" << std::endl;
    
    return 0;
}

#ifdef WASM
extern "C" {
    int EMSCRIPTEN_KEEPALIVE add (int x, int y) {
        return x + y;
    }

    const char* EMSCRIPTEN_KEEPALIVE patch (
        const char *os, 
        char osHashPrefix, 
        const char *ts, 
        char tsHashPrefix
    ) {
        std::shared_ptr<vDOM> o(new vDOM(os, osHashPrefix));
        std::shared_ptr<vDOM> t(new vDOM(ts, tsHashPrefix));
        return o->to(t)->toJson().c_str();
    }
}
#endif
