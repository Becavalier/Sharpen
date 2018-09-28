import Core from 'sharpen/core';

class SharpenWrapper {
    constructor () {
        this.coreIns = null;
        this.initialized = false;
    }

    init (path) {
        if (!this.initialized) {
            new Core({
                locateFile: f => (path + f)
            }, instance => {
                this.coreIns = instance;
                this.initialized = true;
            });
        }
    }

    getInstance () {
        if (this.initialized) {
            return this.coreIns;
        } else {
            throw '[Sharpen] please initialize it first!';
        }
    }
}

export default new SharpenWrapper();
