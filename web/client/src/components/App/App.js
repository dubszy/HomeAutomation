import React from 'react'

import Sidebar from 'c/Sidebar'

import Column from 'c/Column'
import Header from 'c/Header'
import MultiColumn from 'c/MultiColumn'
import MultiRow from 'c/MultiRow'
import NetworkManagement from 'c/page/NetworkManagement'
import Row from 'c/Row'

import classes from './App.css'

export default class App extends React.Component {

  constructor() {
    super();
  }

  render() {
    return (
      <div className='app'>
        <MultiRow id='superMultiRow'>
          <Row
            minHeight='50px'
            maxHeight='50px'
          >
            <Header>
              Home Automation
            </Header>
          </Row>
          <Row>
            <MultiColumn>
              <Column flex={1}>
                <Sidebar/>
              </Column>
              <Column flex={3}>
                <NetworkManagement/>
              </Column>
            </MultiColumn>
          </Row>
        </MultiRow>
      </div>
    )
  }
}