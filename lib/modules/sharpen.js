// flags;
const vDOMType = {
    RELAY: 1 << 1,
    ENDPOINT: 1 << 2,
    EMPTY: 1 << 3
};

export default class Sharpen {
    constructor () {
        this.version = "0.0.1";
        this.hashCounter = 0;
        this.DOMRefsTable = {};
    }

    hash () {
        return 1 << (this.hashCounter++);
    }

    // must be no "side-effect";
    toJsonVDOM (node, vDOM = {}, deepth = 0) {
        if (deepth === 0){
            this.hashCounter = 0;
        }
        if (node instanceof HTMLElement) {
            vDOM.tagName = node.tagName;
            vDOM.hash = this.hash();
            vDOM.attributes = {};
            Array.prototype.slice.call(node.attributes).forEach(attr => {
                vDOM.attributes[attr.nodeName] = attr.nodeValue;
            });
            this.DOMRefsTable[vDOM.hash] = node;
            if (node.children.length > 0) {
                vDOM.children = {};
                for (let e of node.children) {
                    this.toJsonVDOM(e, vDOM.children, ++deepth);
                }
                vDOM.type = vDOMType.RELAY;
            } else if (node.innerText) {
                vDOM.innerText = node.innerText;
                vDOM.type = vDOMType.ENDPOINT;
            } else {
                vDOM.type = vDOMType.EMPTY;
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
