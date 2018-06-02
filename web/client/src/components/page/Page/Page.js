import React from 'react'

export default class Page extends React.Component {

  constructor() {
    super();
  }

  render() {
    const { children } = this.props;

    return (
      <div>
        {children}
      </div>
    )
  }
}