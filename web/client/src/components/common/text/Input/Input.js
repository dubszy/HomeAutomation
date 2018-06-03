import React from 'react'
import PropTypes from 'prop-types'

import classes from './Input.css'

export default class Input extends React.Component {

  constructor() {
    super();
    this.state = {
      value: ''
    }
  }

  render() {
    const { className, onChange, ...restProps } = this.props;

    let classNames = (typeof className !== 'undefined') ? className : '';
    classNames += ' cInput';

    return (
      <input
        className={classNames}
        onChange={() => onChange}
        {...restProps}
      />
    )
  }
};

Input.propTypes = {
  onChange: PropTypes.func
};