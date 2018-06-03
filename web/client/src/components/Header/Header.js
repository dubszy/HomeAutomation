import React from 'react'

import classes from './Header.css'

export default class Header extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <div className='header'>
        {children}
      </div>
    );
  }
}