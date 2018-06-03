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

  static processLabelProps(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      ...rest
    } = props;

    classNames.push('labelForTextArea');

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

  static processTextAreaProps(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      ...rest
    } = props;

    if (typeof className !== 'undefined') {
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

  static propcessor(props) {

    const styles = {};
    const classNames = [];

    const {
      className,
      ...rest
    } = props;

    classNames.push('textAreaWithLabel');

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
    const { labelText, labelProps, textAreaProps, ...rest } = this.props;
    const processedLabelProps = TextAreaWithLabel.processLabelProps(labelProps || {});
    const processedTextAreaProps = TextAreaWithLabel.processTextAreaProps(textAreaProps || {});
    const processedProps = TextAreaWithLabel.propcessor(rest);

    return (
      <div {...processedProps}>
        <label {...processedLabelProps}>
          {labelText}
          <div className='labelTextArea'>
            <TextArea {...processedTextAreaProps}/>
          </div>
        </label>
      </div>
    )
  }
};

TextAreaWithLabel.propTypes = {
  labelText: PropTypes.string.isRequired
};