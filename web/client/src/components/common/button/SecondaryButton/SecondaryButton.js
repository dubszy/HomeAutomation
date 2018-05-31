
import React from 'react'

import Button from 'c/common/button/Button'

export default class SecondaryButton extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { onClick, text } = this.props;

    return (
      <Button
        type='SECONDARY'
        onClick={onClick}
        text={text}
      />
    );
  }
}