#include <chrono>
#include "gtest/gtest.h"
#include "lib/vdom/vdom.h"

using sharpen_vdom::vDOM;

TEST(Sharpen, VDOM) {
    auto start = std::chrono::high_resolution_clock::now();

    // test Json parser;
    std::string os = "{\
        \"tagName\": \"DIV\",\
        \"hash\": \"o_0\",\
        \"attributes\": {\
            \"id\": \"native\",\
            \"data-tid\": \"apple\",\
            \"data-type\": \"div\"\
        },\
        \"children\": {\
            \"o_1\": {\
                \"tagName\": \"SPAN\",\
                \"hash\": \"o_1\",\
                \"attributes\": {\
                    \"style\": \"font-size: 12px; font-weight: bold;\"\
                },\
                \"innerText\": \"Apple\",\
                \"type\": 4\
            },\
            \"o_2\": {\
                \"tagName\": \"H4\",\
                \"hash\": \"o_2\",\
                \"attributes\": {},\
                \"innerText\": \"A very cool company!\",\
                \"type\": 4\
            }\
        },\
        \"type\": 2\
    }";

    std::string ts = "{\
        \"tagName\": \"DIV\",\
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
                \"tagName\": \"SPAN\",\
                \"hash\": \"t_1\",\
                \"attributes\": {\
                    \"style\": \"font-size: 20px; color: blue; font-weight: bold;\"\
                },\
                \"innerText\": \"Google\",\
                \"type\": 4\
            },\
            \"t_2\": {\
                \"tagName\": \"P\",\
                \"hash\": \"t_2\",\
                \"attributes\": {},\
                \"innerText\": \"An amazing company!\",\
                \"type\": 4\
            },\
            \"t_3\": {\
                \"tagName\": \"P\",\
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
    auto diff = (o->to(t))->toJson();

    // elapsed time;
    std::chrono::duration<double> elapsed = (std::chrono::high_resolution_clock::now() - start);
    std::cout << "Analysis time (diff): " 
              << elapsed.count() * 1000
              << " ms\n";
}
