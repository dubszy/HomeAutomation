import React from 'react'

import Page from 'c/page/Page'

import classes from './Nodes.css'

export default class Nodes extends React.Component {

  constructor() {
    super();
  }

  render() {

    return (
      <Page>
        <p>
          Status and settings for all nodes on your home automation network. These are the nodes that control your
          devices; for the status and settings of your devices, see the Devices page.
        </p>
      </Page>
    );
  }
}