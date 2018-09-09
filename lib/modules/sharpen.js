// flags;

// commitActions
const _U_ = 1 << 1;  // 2;
const _C_ = 1 << 2;  // 4;
const _D_ = 1 << 3;  // 8;

// commitTypes;
const _HTML_ = 1 << 1;  // 2;
const _ATTRIBUTE_ = 1 << 1;  // 4;

// commitPayload;
const _CP_ACT_ = 1 << 1;  // 2;
const _CP_TYP_ = 1 << 2;  // 4;
const _CP_KEY_ = 1 << 3;  // 8;
const _CP_HAS_ = 1 << 4;  // 16;
const _CP_VAL_ = 1 << 5;  // 32;

// commitActions
const _DOM_TYPE_RELAY_ = 1 << 1;  // 2;
const _DOM_TYPE_ENDPOINT_ = 1 << 2;  // 4;
const _DOM_TYPE_EMPTY_ = 1 << 3;  // 8;


export default class Sharpen {
    constructor () {
        this.version = "0.0.1";
        this.hashCounter = 0;
        this.hashBlock = -1;
        this.DOMRefsTable = {};
    }

    hash () {
        return 1 << (this.hashCounter++);
    }

    // must be no "side-effect";
    toJsonVDOM (node, hash = 0, vDOM = {}, deepth = 0) {
        if (deepth === 0){
            this.hashBlock++;
            this.hashCounter = 1;
        }
        if (node instanceof HTMLElement) {
            vDOM.tagName = node.tagName;
            vDOM.hash = hash;
            vDOM.attributes = {};
            Array.prototype.slice.call(node.attributes).forEach(attr => {
                vDOM.attributes[attr.nodeName] = attr.nodeValue;
            });
            this.DOMRefsTable[`${this.hashBlock}.${vDOM.hash}`] = node;
            if (node.children.length > 0) {
                let _hash = this.hash();
                vDOM.children = {
                    [_hash]: {}
                };
                for (let e of node.children) {
                    this.toJsonVDOM(e, _hash, vDOM.children[_hash], ++deepth);
                }
                vDOM.type = _DOM_TYPE_RELAY_;
            } else if (node.innerText) {
                vDOM.innerText = node.innerText;
                vDOM.type = _DOM_TYPE_ENDPOINT_;
            } else {
                vDOM.type = _DOM_TYPE_EMPTY_;
            }
        }

        return JSON.stringify(vDOM);
    }
    
    mallocStr (str) {
        // '\0';
        let size = Module['lengthBytesUTF8'](str) + 1;
        let ptr = Module['_malloc'](size);
        Module['stringToUTF8'](str, ptr, size);
        return ptr;
    }

    reflect (diffSeqs) {
        console.log(diffSeqs);
    }

    // original / to;
    patch (oDOM, tDOM) {
        let oPtr = this.mallocStr(this.toJsonVDOM(oDOM));
        let tPtr = this.mallocStr(this.toJsonVDOM(tDOM));
        
        // call core diff algorithm;
        let diffPtr = Module['_patch'](oPtr, tPtr);

        // clean;
        Module['_free'](oPtr);
        Module['_free'](tPtr);

        // result and reflect;
        let diffSequences = JSON.parse(Module['Pointer_stringify'](diffPtr));
        this.reflect(diffSequences);
    }
};
