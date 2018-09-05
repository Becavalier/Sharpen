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
    toJsVDOM (node, vDOM = {}, deepth = 0) {
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
                    this.toJsVDOM(e, vDOM.children, ++deepth);
                }
            }
        }

        return vDOM;
    }

    toCppVDOM (jsVDOM) {
        // reduce overhead between "Wasm" and "JavaScript";
        let str = JSON.stringify(jsVDOM);
        console.log(str);
    }

    patch (natvieDOM, patchDOM) {
        this.toCppVDOM(this.toJsVDOM(natvieDOM));
    }
};
