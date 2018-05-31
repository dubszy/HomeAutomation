import React from 'react'

import Button from 'c/common/button/Button'

export default class CautionButton extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { onClick, text } = this.props;

    return (
      <Button
        type='CAUTION'
        onClick={onClick}
        text={text}
      />
    );
  }
}