import { createStore, applyMiddleware, compose } from 'redux'
import createHistory from 'history/createBrowserHistory'
import { routerMiddleware } from 'react-router-redux'
import thunk from 'redux-thunk'

import { reducer } from './reducers/CombinedReducer'

const buildEnhancer = () => {

  if (process.env.NODE_ENV !== 'production') {
    return compose(
      applyMiddleware(
        thunk,
        routerMiddleware(history)
      ),
      window.devToolsExtension ? window.devToolsExtension() : f => f
    );
  } else {
    return applyMiddleware(
      thunk,
      routerMiddleware(history)
    );
  }
};

export function buildHistory() {
  return createHistory();
}

export function buildStore(initialState = {}) {
  return createStore(reducer, initialState, buildEnhancer());
}

export const history = buildHistory();
export const store = buildStore();