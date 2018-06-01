import React from 'react'
import PropTypes from 'prop-types'

import classes from './Input.css'

export default class Input extends React.Component {

  constructor() {
    super();
    this.handleOnChange = this.handleOnChange.bind(this);
    this.state = {
      value: ''
    }
  }

  render() {
    const { onChange, ...restProps } = this.props;
    return (
      <input
        className={classes.cInput}
        onChange={() => onChange}
        {...restProps}
      />
    )
  }
};

Input.propTypes = {
  onChange: PropTypes.func
};