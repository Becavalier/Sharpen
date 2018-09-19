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

    // tes ^Map;
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


    // test json parser;
    std::string os = "{                             \
        'tagName':'DIV',                            \
        'hash':1-0,                                 \
        'attributes':{                              \
            'id':'native',                          \
            'data-tid':'apple'                      \
        },                                          \
        'children':{                                \
            '1-1':{                                 \
                'tagName':'SPAN',                   \
                'hash':1-1,                         \
                'innerText':'Apple',                \
                'type':4                            \
            }                                       \
        },                                          \
        'type':2                                    \
    }";

    std::string ts = "{                             \
        'tagName':'DIV',                            \
        'hash':2-0,                                 \
        'attributes':{                              \
            'id':'patch',                           \
            'data-sid':'google',                    \
            'onclick':'alert(true);',               \
            'style':'background-color: yellow;'     \
        },                                          \
        'children':{                                \
            '2-1':{                                 \
                'tagName':'SPAN',                   \
                'hash':2-1,                         \
                'innerText':'Google',               \
                'type':4,                           \
                'attributes':{                      \
                    'style':'font-size:20px;'       \
                }                                   \
            }                                       \
        },                                          \
        'type':2                                    \
    }";

    auto jsonRes = std::make_shared<RSJresource>(os);
    std::shared_ptr<Map> m = Util::DCP<Map>(jsonRes->parseAll());


    // test vdom && diff;
    std::shared_ptr<vDOM> o(new vDOM(os, '1'));
    std::shared_ptr<vDOM> t(new vDOM(ts, '2'));
    auto diff = (o->to(t))->toJson();


    // test LD algorithm;
    std::vector<std::string> v1{"d", "e", "m", "o", "c", "r", "a", "t"};
    std::vector<std::string> v2{"r", "e", "p", "u", "b", "l", "i", "c", "a", "n"};
    auto result = Util::findLevenshteinDistancePath(v1, v2);
    Util::applyLDResult(v1, v2, result);
    Util::print(v1);


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
        return (o->to(t))->toJson().c_str();
    }
}
#endif