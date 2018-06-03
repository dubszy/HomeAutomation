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
    const { className, onChange, ...restProps } = this.props;

    let classNames = (typeof className !== 'undefined') ? className : '';
    classNames += ' textArea';

    return (
      <textarea
        className={classNames}
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