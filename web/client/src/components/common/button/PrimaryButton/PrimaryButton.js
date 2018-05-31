import React from 'react'

import Button from 'c/common/button/Button'

export default class PrimaryButton extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { onClick, text } = this.props;

    return (
      <Button
        type='PRIMARY'
        onClick={onClick}
        text={text}
      />
    );
  }
}