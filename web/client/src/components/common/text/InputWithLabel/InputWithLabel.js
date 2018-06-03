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

  static processLabelProps(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      ...rest
    } = props;

    classNames.push('labelForInput');

    if (typeof className !== 'undefined') {
      classNames.push(className);
    }

    return Object.assign(
      {},
      {
        style: styles,
        className: classNames.join(' ')
      },
      rest
    );
  }

  static processInputProps(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      cssFloat,
      ...rest
    } = props;

    if (typeof className !== 'undefined') {
      classNames.push(className)
    }

    if (typeof cssFloat !== 'undefined') {
      styles.cssFloat = cssFloat;
    }

    return Object.assign(
      {},
      {
        style: styles,
        className: classNames
      },
      rest
    );
  }

  static propcessor(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      ...rest
    } = props;

    classNames.push('inputWithLabel');

    if (className) {
      classNames.push(className);
    }

    return Object.assign(
      {},
      {
        className: classNames.join(' '),
        style: styles
      },
      rest
    );
  }

  render() {
    const { labelText, labelProps, inputProps, ...rest } = this.props;
    const processedLabelProps = InputWithLabel.processLabelProps(labelProps || {});
    const processedInputProps = InputWithLabel.processInputProps(inputProps || {});
    const processedProps = InputWithLabel.propcessor(rest);

    return (
      <div {...processedProps}>
        <label {...processedLabelProps}>
          {labelText}
          <Input {...processedInputProps}/>
        </label>
      </div>
    )
  }
};

InputWithLabel.propTypes = {
  labelText: PropTypes.string.isRequired,
  onChange: PropTypes.func
};