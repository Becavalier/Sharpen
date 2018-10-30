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
    std::string os = u8R"({
        "1": {
            "id": "native",
            "data-tid": "apple",
            "data-type": "div"
        },
        "2": {
            "o1": {
                "1": {
                    "style": "font-size: 12px; font-weight: bold;"
                },
                "3": "o1",
                "4": "SPAN",
                "5": 2,
                "6": "Apple"
            },
            "o2": {
                "3": "o2",
                "4": "H4",
                "5": 2,
                "6": "A very cool company!"
            }
        },
        "3": "o0",
        "4": "DIV",
        "5": 1
    })";

    std::string ts = u8R"({
        "1": {
            "id": "patch",
            "onclick": "alert(true);",
            "style": "background-color: yellow;",
            "data-sid": "google",
            "data-type": "div"
        },
        "2": {
            "t1": {
                "1": {
                    "style": "font-size: 20px; color: blue; font-weight: bold;"
                },
                "3": "t1",
                "4": "SPAN",
                "5": 2,
                "6": "谷歌"
            },
            "t2": {
                "3": "t2",
                "4": "P",
                "5": 2,
                "6": "一家伟大的公司!"
            },
            "t3": {
                "1": {
                    "style": "color: red;"
                },
                "3": "t3",
                "4": "P",
                "5": 2,
                "6": "share: $1164.83"
            }
        },
        "3": "t0",
        "4": "DIV",
        "5": 1
    })";

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
