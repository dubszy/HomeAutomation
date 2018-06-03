import React from 'react'

import classes from './Page.css'

export default class Page extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <div className='page'>
        {children}
      </div>
    )
  }
}