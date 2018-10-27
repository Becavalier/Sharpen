import SharpenWasm from 'SharpenWasm';
// flags;

// VDOMElements;
const _VDOM_ATTRIBUTES_ = 1;
const _VDOM_CHILDREN_ = 2;
const _VDOM_HASH_ = 3;
const _VDOM_TAGNAME_ = 4;
const _VDOM_TYPE_ = 5;
const _VDOM_INNERTEXT_ = 6;
// domTypes;
const _DOM_TYPE_RELAY_ = 1;
const _DOM_TYPE_ENDPOINT_ = 2;
const _DOM_TYPE_EMPTY_ = 3;

// commitActions
const _U_ = 1;
const _C_ = 2;
const _D_ = 3;
// commitTypes;
const _HTML_ = 1;
const _ATTRIBUTE_ = 2;
const _INNER_TEXT_ = 3;
const _STYLE_ = 4;
// commitPayload;
const _CP_ACT_ = 1;
const _CP_TYP_ = 2;
const _CP_KEY_ = 3;
const _CP_HAS_ = 4;
const _CP_VAL_ = 5;

export default class Sharpen {
    constructor (config, cb) {
        // extern;
        SharpenWasm(config).then(ins => {
            this.core = ins;
            cb && cb(this);
        });

        this.version = '0.0.1';
        this.hashCounter = 0;
        this.DOMRefsTable = {};
        this.enableDeepClone = true;
        this.core = false;
    }

    hash () {
        return this.hashCounter++;
    }

    // must be no "side-effect";
    toJsonVDOM (node, hashBlock, hash = 0, vDOM = {}, depth = 0) {
        this.hashBlock = hashBlock;
        if (depth === 0){
            this.hashCounter = 1;
        }
        if (node instanceof HTMLElement) {
            vDOM[_VDOM_TAGNAME_] = node.tagName;
            vDOM[_VDOM_HASH_] = `${hashBlock}${hash}`;
            vDOM[_VDOM_ATTRIBUTES_] = {};
            Array.prototype.slice.call(node.attributes).forEach(attr => {
                vDOM[_VDOM_ATTRIBUTES_][attr.nodeName] = attr.nodeValue;
            });
            this.DOMRefsTable[vDOM[_VDOM_HASH_]] = node;
            if (node.children.length > 0) {
                vDOM[_VDOM_CHILDREN_] = vDOM[_VDOM_CHILDREN_] ? vDOM[_VDOM_CHILDREN_] : {};
                for (let e of node.children) {
                    let _hash = this.hash();
                    let _hash_all = `${hashBlock}${_hash}`;
                    vDOM[_VDOM_CHILDREN_][_hash_all] = {};
                    this.toJsonVDOM(e, hashBlock, _hash, vDOM[_VDOM_CHILDREN_][_hash_all], ++depth);
                }
                vDOM[_VDOM_TYPE_] = _DOM_TYPE_RELAY_;
            } else if (node.innerText) {
                vDOM[_VDOM_INNERTEXT_] = node.innerText;
                vDOM[_VDOM_TYPE_] = _DOM_TYPE_ENDPOINT_;
            } else {
                vDOM[_VDOM_TYPE_] = _DOM_TYPE_EMPTY_;
            }
        }

        return JSON.stringify(vDOM);
    }
    
    mallocStr (str) {
        // +'\0';
        let size = this.core['lengthBytesUTF8'](str) + 1;
        let ptr = this.core['_malloc'](size);
        this.core['stringToUTF8'](str, ptr, size);
        return ptr;
    }

    setupHTMLElement (vDOM, parent = false) {
        let node = this.DOMRefsTable[vDOM[_VDOM_HASH_]];
        if (node && this.enableDeepClone) {
            return node.cloneNode(true);
        } else {
            node = document.createElement(vDOM[_VDOM_TAGNAME_]); 
            for (let key in vDOM[_VDOM_ATTRIBUTES_]) {
                node.setAttribute(key, vDOM[_VDOM_ATTRIBUTES_][key]);
            }
            if (vDOM[_VDOM_CHILDREN_]) {
                for (let child in vDOM[_VDOM_CHILDREN_]) {
                    this.setupHTMLElement(child, node);
                }
            }
            if (vDOM[_VDOM_TYPE_] == _DOM_TYPE_ENDPOINT_) {
                node.appendChild(document.createTextNode(vDOM[_VDOM_INNERTEXT_]));
            }
            if (parent) {
                parent.appendChild(node);
            }
        }
        return node;
    }

    reflect (diffSeqs) {
        diffSeqs.forEach(commit => {
            let ref = this.DOMRefsTable[commit[_CP_HAS_]];
            // [attributes];
            if (commit[_CP_TYP_] === _ATTRIBUTE_) {
                if (commit[_CP_ACT_] === _U_ || commit[_CP_ACT_] === _C_) {
                    // update or add;
                    ref.setAttribute(commit[_CP_KEY_], commit[_CP_VAL_]);
                } else if (commit[_CP_ACT_] === _D_) {
                    // delete;
                    ref.removeAttribute(commit[_CP_VAL_]);
                }
            }

            // [html];
            if (commit[_CP_TYP_] === _HTML_) {
                if (commit[_CP_ACT_] === _U_) {
                    ref.parentNode.replaceChild(this.setupHTMLElement(commit[_CP_VAL_]), ref);
                } else if (commit[_CP_ACT_] === _C_) {
                    ref.appendChild(this.setupHTMLElement(commit[_CP_VAL_]));
                } else if (commit[_CP_ACT_] === _D_) {
                    // delete;
                    ref.remove();
                }
            }

            // [innerText];
            if (commit[_CP_TYP_] === _INNER_TEXT_) {
                if (commit[_CP_ACT_] === _U_ || commit[_CP_ACT_] === _C_) {
                    ref.innerText = commit[_CP_VAL_];
                } else if (commit[_CP_ACT_] === _D_) {
                    ref.innerText = '';
                }
            }

            // [style];
            if (commit[_CP_TYP_] === _STYLE_) {
                if (commit[_CP_ACT_] === _U_ || commit[_CP_ACT_] === _C_) {
                    ref.style.setProperty(commit[_CP_KEY_], commit[_CP_VAL_]);
                } else if (commit[_CP_ACT_] === _D_) {
                    ref.style.removeProperty(commit[_CP_VAL_]);
                }
            }
        });
    }

    // original / to;
    patch (oDOM, tDOM) {
        if (!oDOM || !tDOM) {
            console.error('[Sharpen] invalid DOM reference!');
            return;
        }
        // performance log start;
        const startTime = performance.now();

        const oJsonVDOM = this.toJsonVDOM(oDOM, 'o');
        const tJsonVDOM = this.toJsonVDOM(tDOM, 't');
        
        if (oJsonVDOM !== tJsonVDOM) {
            let op = this.mallocStr(oJsonVDOM);
            let tp = this.mallocStr(tJsonVDOM);
            
            // call diff algorithm;
            let diffPtr = this.core['_patch'](op, 111, tp, 116);

            // reflect the changes;
            let diffSequences = JSON.parse(this.core['Pointer_stringify'](diffPtr));
            this.reflect(diffSequences);

            // performance log end;
            console.info(`[Sharpen] render time used: ${(performance.now() - startTime).toFixed(3)}ms`);

            // clean memory async;
            this.core['_free'](op);
            this.core['_free'](tp);
        }
    }
}
