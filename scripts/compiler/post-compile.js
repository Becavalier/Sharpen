const fs = require("fs");
const path = require("path");
const webpack = require("webpack");
const MemoryFS = require("memory-fs");
const replace = require('replace-in-file');

const MODULE_FILES_PATH = "lib/modules";
const ORIGINAL_SCRIPT_PATH = "dist/sharpen.min.js"
// const DEBUG_SCRIPT_PATH = "dist/sharpen.debug.min.js";

const config = {
    entry: path.resolve(__dirname, "../..", MODULE_FILES_PATH),
    mode: "production",
    target: "web",
    module: {
        rules: [
            {
                test: /\.js$/,
                loader: 'babel-loader'
            }
        ]
    },
    output: {
        path: "/",
        filename: "__t.js",
        library: "__sharpen",
        libraryTarget: "var", 
    },
};

const compiler = webpack(config);
const mfs = new MemoryFS();
compiler.outputFileSystem = mfs;

// run webpack;
compiler.run((err, stats) => {
    if (err) throw err;
    const content = mfs.readFileSync("/__t.js");
    // clear first;
    replace({
        files: ORIGINAL_SCRIPT_PATH,
        from: /\/\*_POST_START_\*\/(.*)\/\*_POST_END_\*\/$/g,
        to: '',
    }).then(changes => {
        // merge;
        fs.appendFile(ORIGINAL_SCRIPT_PATH, 
            "/*_POST_START_*/__ATPOSTRUN__.push(function() {" + content + ";Module.sharpen=__sharpen.default;});/*_POST_END_*/", 
            (err) => {
                if (err) throw err;
                console.log("[Sharpen] Complete!");
            });
    });
});
