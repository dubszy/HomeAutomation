import React from 'react'
import { Route } from 'react-router'

import Sidebar from 'c/Sidebar'

import Column from 'c/Column'
import Dashboard from 'c/page/Dashboard'
import Devices from 'c/page/Devices'
import Header from 'c/Header'
import MultiColumn from 'c/MultiColumn'
import MultiRow from 'c/MultiRow'
import NetworkManagement from 'c/page/NetworkManagement'
import Nodes from 'c/page/Nodes'
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
                <div>
                  <Route path='/dashboard' component={Dashboard}/>
                  <Route path='/devices' component={Devices}/>
                  <Route path='/network' component={NetworkManagement}/>
                  <Route path='/nodes' component={Nodes}/>
                </div>
              </Column>
            </MultiColumn>
          </Row>
        </MultiRow>
      </div>
    )
  }
}