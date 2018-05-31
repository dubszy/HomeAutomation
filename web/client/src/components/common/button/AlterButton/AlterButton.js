import React from 'react'

import Button from 'c/common/button/Button'

export default class AlterButton extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { onClick, text } = this.props;

    return (
      <Button
        type='ALTER'
        onClick={onClick}
        text={text}
      />
    );
  }
}