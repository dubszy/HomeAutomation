import React from 'react'

import classes from './UnorderedList.css'

export default class UnorderedList extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <ul
        className={classes.unorderedList}
      >
        {children}
      </ul>
    );
  }
}