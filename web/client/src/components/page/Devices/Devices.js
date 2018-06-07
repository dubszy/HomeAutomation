import React from 'react'

import Page from 'c/page/Page'

import classes from './Devices.css'

export default class Devices extends React.Component {

  constructor() {
    super();
  }

  render() {

    return (
      <Page>
        <p>
          Status and settings for all devices on your home automation network. These are the devices that your nodes
          control; for the status and settings of your nodes, see the Nodes page.
        </p>
      </Page>
    )
  }
}