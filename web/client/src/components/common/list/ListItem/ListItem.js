import React from 'react'

import classes from './ListItem.css'

export default class ListItem extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <li
        className={classes.listItem}
      >
        { children }
      </li>
    )
  }
}