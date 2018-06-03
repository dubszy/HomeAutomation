import React from 'react'
import PropTypes from 'prop-types'

import Input from 'c/common/text/Input'

import classes from './InputWithLabel.css'

/**
 * This component is comprised of an Input component inside of a label element, inside of a div.
 * Text is required for the label. If just an Input is needed, use the Input component.
 */
export default class InputWithLabel extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { labelText, onChange, ...restProps } = this.props;

    return (
      <div className='inputWithLabel'>
        <label>
          {labelText}
          <Input
            className='labelInput'
            onChange={onChange}
            {...restProps}
          />
        </label>
      </div>
    )
  }
};

InputWithLabel.propTypes = {
  labelText: PropTypes.string.isRequired,
  onChange: PropTypes.func
};