import React from 'react'

import InputWithLabel from 'c/common/text/InputWithLabel'
import Page from 'c/page/Page'
import TextAreaWithLabel from "../../common/text/TextAreaWithLabel/TextAreaWithLabel";

import classes from './NetworkManagement.css'

export default class NetworkManagement extends React.Component {

  constructor() {
    super();
  }

  render() {

    return (
      <Page>
        <p>
          Enter information about your local network on this page. This does not affect settings on your router. The
          purpose of these settings is to provide the home automation network with information about how your local
          network is set up.
        </p>
        <div className='networkSettingsGroup'>
          <InputWithLabel
            labelText='Gateway IP Address'
            inputProps={{
              cssFloat: 'right',
              placeholder: 'Ex. 10.0.0.1  192.168.0.1'
            }}
          />
          <InputWithLabel
            labelText='Subnet Mask'
            inputProps={{
              cssFloat: 'right',
              placeholder: 'Ex. 255.255.255.0'
            }}
          />
          <InputWithLabel
            labelText='Web Server Domain Name'
            inputProps={{
              cssFloat: 'right',
              placeholder: 'Ex. web.example.com'
            }}
          />
        </div>
        <TextAreaWithLabel
          labelText='Ignore the following IP addresses when polling for nodes'
          textAreaProps={{
            cols: 40
          }}
        />
      </Page>
    )
  }
}