import React from 'react'

import Page from 'c/page/Page'

import classes from './Dashboard'

export default class Dashboard extends React.Component {

  constructor() {
    super();
  }

  render() {

    return (
      <Page>
        <p>
          Top-level overview of your home automation setup.
        </p>
      </Page>
    );
  }
}