'use strict';

let express = require('express');
let bodyParser = require('body-parser');

let env = require('../../config/env.json');

const app = express();
let ready = false;

app.use(bodyParser.json());
app.use(express.static('client'));

app.listen(env.server.port, () => {
    ready = true;
    console.log('Listening on port ' + env.server.port);
});

app.all('*', (req, res, next) => {
    if (!ready) {
        return res.status(503).json({success: false, message: 'Server is not ready yet', data: {}});
    }
    next();
});

app.get('*', (req, res, next) => {
  let path = req.path;
  if (path.indexOf('/api') === 0 ||
      path.indexOf('/favicon.ico') === 0) {
    next();
  } else {
    let options = {
      root: 'client/src/',
      headers: {
        'contentType': 'text/html'
      }
    };

    res.sendFile('index.html', options, (err) => {
      if (err) {
        let msg = 'Could not find index.html';
        console.error(msg, err);
        return res.status(404).json({success: false, message: msg, data: {}}).end();
      }
    })
  }
});

app.get('/', (req, res) => {
    let options = {
        root: 'client/src/',
        headers: {
            'contentType': 'text/html'
        }
    };

    res.sendFile('index.html', options, (err) => {
        if (err) {
            let msg = 'Could not find index.html';
            console.error(msg, err);
            return res.status(404).json({success: false, message: msg, data: {}}).end();
        }
    })
});

// TODO: Create a favicon
app.get('/favicon.ico', (req, res) => {
    console.log('favicon not implemented');
    return res.status(501).end('Favicon not implemented');
});