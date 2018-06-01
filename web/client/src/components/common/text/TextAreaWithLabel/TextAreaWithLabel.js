import React from 'react'
import PropTypes from 'prop-types'

import TextArea from 'c/common/text/TextArea';

import classes from './TextAreaWithLabel.css'

/**
 * This component is comprised of a TextArea component inside of a label element, inside of a div.
 * Text is required for the label. If just a TextArea is needed, use the TextArea component.
 */
export default class TextAreaWithLabel extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { labelText, onChange, ...restProps } = this.props;

    return (
      <div className={classes.textAreaWithLabel}>
        <label>
          {labelText}
          <TextArea
            onChange={onChange}
            {...restProps}
          />
        </label>
      </div>
    )
  }
};

TextAreaWithLabel.propTypes = {
  labelText: PropTypes.string.isRequired
};