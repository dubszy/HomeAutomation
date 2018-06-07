let path = require('path');

const config = {
    entry: [
        './src/AppEntry.js'
    ],
    output: {
        path: __dirname + "/dist",
        filename: "bundle.js"
    },
    resolve: {
        alias: {
            c: path.resolve(__dirname, 'src/components')
        }
    },
    module: {
        loaders: [
            {test: /\.js/, exclude: /node_modules/, loader: 'babel-loader', query: {"presets": ["es2016", "stage-2", "react"]}},
            {test: /\.css$/, loader: "style-loader!css-loader"}
        ]
    }
};

module.exports = config;