# ⚡️Sharpen

[![experimental](http://badges.github.io/stability-badges/dist/experimental.svg)](http://github.com/badges/stability-badges)

> A v-dom "diff" engine based on WebAssembly, aim to build efficient and fluent web apps.

## Basic Concept

We use the following kind of data structure to pass the "Diff" info of input DOMs from the C++ side to JavaScript side. All the data exchange through these two contexts will be encoded and decoded as standard JSON format by our lightweight specialized JSON parser (Not a standard full-parser). Here we can call the following data structure is "**`DDIR`**". Each "Object" structure shows in the following "Array" stucture will be regared as a diff "**`Commit`**", and those commits will be transacted and inflect on the real DOM by the specific JavaScript code.

```
DDIR:
[
    ...,
    {
        _CP_ACT_: _D_,
        _CP_TYP_: _HTML_,
        _CP_VAL_: TypeRoot*
    },
    {
        _CP_ACT_: _U_,
        _CP_TYP_: _ATTR_,
        _CP_HAS_: TypeRoot*,
        _CP_KEY_: TypeRoot*,
        _CP_VAL_: TypeRoot*
    },
    {
        _CP_ACT_: _C_,
        _CP_TYP_: _TEXT_,
        _CP_HAS_: TypeRoot*,
        _CP_VAL_: TypeRoot*
    },
    {
        _CP_ACT_: _U_,
        _CP_TYP_: _STYLE_,
        _CP_HAS_: "",
        _CP_KEY_: TypeRoot*,
        _CP_VAL_: TypeRoot*
    }
    ...
]
```

* **Why do we use JSON?**

Because "JSON" is lightweight and it's easy to encode and decode, also it's better to reduce the overhead between WebAssembly and JavaScrit contexts by this kind of "one-time" data delivery.


## Getting Started

### Compile
If you want to compile and use this project, please install the following software before:

* [Emscripten v1.38.0](https://github.com/kripken/emscripten/releases/tag/1.38.0)
* [cmake v3.11 (or above)](https://cmake.org/install/)

then, run the following command for compiling:

`npm run build`


then, run the demo:

`npm run test`

### Others

* **Code Lint**:

We use "cpplint" to check the code style, you can install it by the follow command:

`pip install cpplint`

And lint the source code by:

`npm run lint`

* **Memory Check**:

Install "valgrind" on MacOS according to the following article first:

*[How to Install Valgrind on macOS High Sierra](https://www.gungorbudak.com/blog/2018/04/28/how-to-install-valgrind-on-macos-high-sierra/)*


Then run the following command to detect the memory leak of the binary version program:

`npm run memcheck`


## Roadmap

- [ ] Add benchmark;
- [ ] Optimize code and fix memory leak;
- [ ] Preact support;
- [ ] GC support;
- [ ] Support "Levenshtein-Distance" search;
- [x] Support partial changes on "style" tag;


## Copyright and License

MIT.