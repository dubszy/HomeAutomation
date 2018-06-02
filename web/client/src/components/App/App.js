import React from 'react'

import Sidebar from 'c/Sidebar'

import classes from './App.css'
import MultiColumn from "../MultiColumn/MultiColumn";
import Column from "../Column/Column";
import NetworkManagement from "../page/NetworkManagement/NetworkManagement";

export default class App extends React.Component {

  constructor() {
    super();
  }

  render() {
    return (
      <div>
        <MultiColumn>
          <Column flex={1}>
            <Sidebar/>
          </Column>
          <Column flex={3}>
            <NetworkManagement/>
          </Column>
        </MultiColumn>
      </div>
    )
  }
}