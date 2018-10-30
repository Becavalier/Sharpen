#include <chrono>
#include "gtest/gtest.h"
#include "lib/vdom/vdom.h"

using sharpen_vdom::vDOM;

TEST(Sharpen, VDOM) {
    auto start = std::chrono::high_resolution_clock::now();

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
    auto diff = (o->to(t))->toJson();

    // elapsed time;
    std::chrono::duration<double> elapsed = (std::chrono::high_resolution_clock::now() - start);
    std::cout << "Analysis time (diff): "
              << elapsed.count() * 1000
              << " ms\n";
}
