import React from 'react'

import classes from './UnorderedList.css'

export default class UnorderedList extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { className, children } = this.props;

    let classNames = (typeof className !== 'undefined') ? className : '';
    classNames += ' unorderedList';

    return (
      <ul
        className={classNames}
      >
        {children}
      </ul>
    );
  }
}