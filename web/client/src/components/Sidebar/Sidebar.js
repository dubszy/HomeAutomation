import React from 'react'

import ListItem from 'c/common/list/ListItem'
import UnorderedList from 'c/common/list/UnorderedList'

import classes from './Sidebar.css'

export default class Sidebar extends React.Component {

  constructor() {
    super();
    this.state = {
      selectedItem: null
    };
    this.onSelectItem = this.onSelectItem.bind(this);
  }

  onSelectItem(itemId) {
    const { updateSelectedItem } = this.props;
    this.setState({
      selectedItem: itemId
    });
    updateSelectedItem(itemId);
  }

  render() {
    return (
      <UnorderedList className='sidebar'>
        <ListItem>
          Network Management
        </ListItem>
      </UnorderedList>
    )
  }
}