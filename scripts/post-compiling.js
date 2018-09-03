const fs = require("fs");
const UglifyJS = require("uglify-js");
const babel = require("babel-core");

const MODULE_FILE_PATH = "./lib/modules/bundle.js";
const DIST_FILE_PATH = "./dist/sharpen.min.js";

// transfrom syntax;
babel.transformFile(MODULE_FILE_PATH, {
    sourceMaps: true
}, function(err, result) {
    if (err) throw err;
    // compression;
    var result = UglifyJS.minify(result.code);
    // merge;
    fs.appendFile(DIST_FILE_PATH, result.code, (err) => {
        if (err) throw err;
        console.log("[Sharpen] Done.");
    });
});
