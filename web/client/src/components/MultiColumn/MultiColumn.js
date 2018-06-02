import React from 'react'

import Column from 'c/Column'

import classes from './MultiColumn.css'

export default class MultiColumn extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <div className='multiColumn'>
        {children}
      </div>
    )
  }
};

MultiColumn.propTypes = {
  children: (props, propName) => {
    const prop = props[propName];
    let error = null;

    React.Children.forEach(prop, (child) => {
      if (child.type !== Column) {
        error = new Error('Children of MultiColumn must be of type Column');
      }
    });

    return error;
  }
};