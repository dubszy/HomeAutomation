import React from 'react'
import { render } from 'react-dom'

import { ConnectedRouter } from 'react-router-redux'
import { Provider } from 'react-redux'

import { history, store } from './Store.js'

import App from 'c/App'

render(
  <Provider store={store}>
    <ConnectedRouter history={history}>
      <App/>
    </ConnectedRouter>
  </Provider>,
  document.getElementById('entry')
);

