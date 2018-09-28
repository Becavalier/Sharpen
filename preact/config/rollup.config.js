import nodeResolve from 'rollup-plugin-node-resolve';
import babel from 'rollup-plugin-babel';
import memory from 'rollup-plugin-memory';
import alias from 'rollup-plugin-alias';
import path from 'path';

export default {
    input: 'src/preact.js',
    output: {
        format: 'iife',
        file: 'dist/preact.dev.js',
        name: 'preact',
        sourcemap: true,
        strict: true,
        globals: {
            SharpenWasm: 'SharpenWasm'
        }
    },
    external: ['SharpenWasm'],
    plugins: [
        memory({
            path: 'src/preact.js',
            contents: `
                import preact from './preact';
                if (typeof module!='undefined') module.exports = preact;
                else self.preact = preact;
            `
        }),
        nodeResolve({
            main: true
        }),
        babel({
            sourceMap: true,
            include: '../core',
            exclude: 'node_modules/**',
            babelrc: false,
            comments: false,
            presets: [
                ['env', {
                    modules: false,
                    loose: true,
                    exclude: ['transform-es2015-typeof-symbol'],
                    targets: {
                        browsers: ['last 2 versions', 'IE >= 9']
                    }
                }]
            ]
        }),
        alias({
            sharpen: path.resolve('..'),
            resolve: ['/index.js'] 
        })
    ]
};
