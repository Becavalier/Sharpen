const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  mode: 'production',
  entry: __dirname + "/index.js",
  output: {
    path: __dirname + "/dist",
    filename: "index.js"
  },
  module: {
    rules: [{
      test: /(\.jsx|\.js)$/,
      use: {
        loader: "babel-loader",
      },
      exclude: /node_modules/
    }]
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: __dirname + "/index.html"
    })
  ],
  resolve: {
    alias: {
      sharpen: path.resolve(__dirname, '../..')
    }
  },
  externals: {
    SharpenWasm: 'SharpenWasm'
  }
}