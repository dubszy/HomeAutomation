import React from 'react'
import PropTypes from 'prop-types'

import classes from './Button.css'

const ButtonType = {
  ALTER: 'ALTER',
  CAUTION: 'CAUTION',
  PRIMARY: 'PRIMARY',
  SECONDARY: 'SECONDARY'
};

export default class Button extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { type, onClick, text } = this.props;

    let classNames = 'button';
    if (type) {
      switch (type) {
        case ButtonType.ALTER:
          classNames += ' buttonAlter';
          break;
        case ButtonType.CAUTION:
          classNames += ' buttonCaution';
          break;
        case ButtonType.PRIMARY:
          classNames += ' buttonPrimary';
          break;
        case ButtonType.SECONDARY:
          classNames += ' buttonSecondary';
          break;
          default:
            console.log('Unrecognized ButtonType: ' + type);
            break;
      }
    }

    return (
      <button
        className={classNames}
        onClick={() => onClick()}
      >
        {text}
      </button>
    );
  }
}

