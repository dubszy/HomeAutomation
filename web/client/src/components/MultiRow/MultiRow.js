import React from 'react'

import Row from 'c/Row'

import classes from './MultiRow.css'

export default class MultiRow extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children, ...rest } = this.props;

    return (
      <div className='multiRow' {...rest}>
        {children}
      </div>
    )
  }
};

MultiRow.propTypes = {
  children: (props, propName) => {
    const prop = props[propName];
    let error = null;

    React.Children.forEach(prop, (child) => {
      if (child.type !== Row) {
        error = new Error('Children of MultiRow must be of type Row');
      }
    });

    return error;
  }
};