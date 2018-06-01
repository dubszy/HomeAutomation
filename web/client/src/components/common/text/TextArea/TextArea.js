import React from 'react'
import PropTypes from 'prop-types'

import classes from './TextArea.css'

export default class TextArea extends React.Component {

  constructor() {
    super();
    this.state = {
      value: ''
    }
  }

  render() {
    const { onChange, ...restProps } = this.props;

    return (
      <textarea
        className={classes.textArea}
        onChange={() => onChange}
        {...restProps}
      />
    );
  }
};

TextArea.defaultProps = {
  cols: '70',
  rows: '10'
};

TextArea.propTypes = {
  onChange: PropTypes.func
};