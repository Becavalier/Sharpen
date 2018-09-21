import SharpenWasm from 'SharpenWasm';
// flags;

// commitActions
const _U_ = 1 << 1;  // 2;
const _C_ = 1 << 2;  // 4;
const _D_ = 1 << 3;  // 8;

// commitTypes;
const _HTML_ = 1 << 1;  // 2;
const _ATTRIBUTE_ = 1 << 2;  // 4;
const _INNER_TEXT_ = 1 << 3;  // 8;
const _STYLE_ = 1 << 4;  // 16;

// commitPayload;
const _CP_ACT_ = 1 << 1;  // 2;
const _CP_TYP_ = 1 << 2;  // 4;
const _CP_KEY_ = 1 << 3;  // 8;
const _CP_HAS_ = 1 << 4;  // 16;
const _CP_VAL_ = 1 << 5;  // 32;

// domTypes;
const _DOM_TYPE_RELAY_ = 1 << 1;  // 2;
const _DOM_TYPE_ENDPOINT_ = 1 << 2;  // 4;
const _DOM_TYPE_EMPTY_ = 1 << 3;  // 8;


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
        this.enableDeepClone = false;
        this.core = false;
    }

    hash () {
        return this.hashCounter++;
    }

    // must be no "side-effect";
    toJsonVDOM (node, hashBlock, hash = 0, vDOM = {}, deepth = 0) {
        this.hashBlock = hashBlock;
        if (deepth === 0){
            this.hashCounter = 1;
        }
        if (node instanceof HTMLElement) {
            vDOM.tagName = node.tagName;
            vDOM.hash = `${hashBlock}_${hash}`;
            vDOM.attributes = {};
            Array.prototype.slice.call(node.attributes).forEach(attr => {
                vDOM.attributes[attr.nodeName] = attr.nodeValue;
            });
            this.DOMRefsTable[vDOM.hash] = node;
            if (node.children.length > 0) {
                vDOM.children = vDOM.children ? vDOM.children : {};
                for (let e of node.children) {
                    let _hash = this.hash();
                    let _hash_all = `${hashBlock}_${_hash}`;
                    vDOM.children[_hash_all] = {};
                    this.toJsonVDOM(e, hashBlock, _hash, vDOM.children[_hash_all], ++deepth);
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
        // +'\0';
        let size = this.core['lengthBytesUTF8'](str) + 1;
        let ptr = this.core['_malloc'](size);
        this.core['stringToUTF8'](str, ptr, size);
        return ptr;
    }

    setupHTMLElement (vDOM, parent = false) {
        let node = this.DOMRefsTable[vDOM[_CP_HAS_]];
        if (node && this.enableDeepClone) {
            return node.cloneNode(true);
        } else {
            node = document.createElement(vDOM.tagName); 
            for (let key in vDOM.attributes) {
                node.setAttribute(key, vDOM.attributes[key]);
            }
            if (vDOM.children) {
                for (let child in vDOM.children) {
                    this.setupHTMLElement(child, node);
                }
            }
            if (vDOM['type'] == _DOM_TYPE_ENDPOINT_) {
                node.appendChild(document.createTextNode(vDOM.innerText));
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
            if (commit[_CP_TYP_] === _ATTRIBUTE_) {
                if (commit[_CP_ACT_] === _U_ || commit[_CP_ACT_] === _C_) {
                    // update or add;
                    ref.setAttribute(commit[_CP_KEY_], commit[_CP_VAL_]);
                } else if (commit[_CP_ACT_] === _D_) {
                    // delete;
                    ref.removeAttribute(commit[_CP_VAL_]);
                }
            }

            if (commit[_CP_TYP_] === _HTML_) {
                if (commit[_CP_ACT_] === _U_) {
                    let parentNode = ref.parentNode;
                    parentNode.replaceChild(this.setupHTMLElement(commit[_CP_VAL_]), ref);
                } else if (commit[_CP_ACT_] === _C_) {
                    ref.appendChild(this.setupHTMLElement(commit[_CP_VAL_]));
                } else if (commit[_CP_ACT_] === _D_) {
                    // delete;
                    ref.remove();
                }
            }

            if (commit[_CP_TYP_] === _INNER_TEXT_) {
                if (commit[_CP_ACT_] === _U_ || commit[_CP_ACT_] === _C_) {
                    ref.innerText = commit[_CP_VAL_];
                } else if (commit[_CP_ACT_] === _D_) {
                    ref.innerText = '';
                }
            }

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

        const startTime = performance.now();

        const oHashPrefix = 'o';
        const tHashPrefix = 't';

        const oJsonVDOM = this.toJsonVDOM(oDOM, oHashPrefix);
        const tJsonVDOM = this.toJsonVDOM(tDOM, tHashPrefix);
       
        if (oJsonVDOM !== tJsonVDOM) {
            let oPtr = this.mallocStr(oJsonVDOM);
            let tPtr = this.mallocStr(tJsonVDOM);
            
            // call core diff algorithm;
            let diffPtr = this.core['_patch'](
                oPtr, oHashPrefix.charCodeAt(), 
                tPtr, tHashPrefix.charCodeAt()
            );

            // clean;
            this.core['_free'](oPtr);
            this.core['_free'](tPtr);

            // result and reflect;
            let diffSequences = JSON.parse(this.core['Pointer_stringify'](diffPtr));
            this.reflect(diffSequences);

            // performance;
            console.info(`[Sharpen] render time used: ${(performance.now() - startTime).toFixed(3)}ms`);
        }
    }
}