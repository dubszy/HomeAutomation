import React from 'react'
import PropTypes from 'prop-types'

import classes from './Column.css'

export default class Column extends React.Component {

  constructor() {
    super();
  }

  static propcessor(props) {
    const styles = {};
    const classNames = [];

    const {
      className,
      flex,
      order,
      minWidth,
      maxWidth,
      zIndex,
      ...rest
    } = props;

    if (className) {
      classNames.push(className);
    }

    if (typeof flex !== 'undefined') {
      styles.flex = flex;
    }

    if (typeof order !== 'undefined') {
      styles.order = order;
    }

    if (typeof minWidth !== 'undefined') {
      styles.minWidth = minWidth;
    }

    if (typeof maxWidth !== 'undefined') {
      styles.maxWidth = maxWidth;
    }

    if (typeof zIndex !== 'undefined') {
      styles.zIndex = zIndex;
    }

    return Object.assign(
      {},
      {
        className: classNames.join(' '),
        style: styles
      },
      rest
    );
  }

  render() {
    const processedProps = Column.propcessor(this.props);

    return (
      <div {...processedProps}>
        {this.props.children}
      </div>
    )
  }
};

Column.propTypes = {
  className: PropTypes.string,
  order: PropTypes.number,
  minWidth: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  maxWidth: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  zIndex: PropTypes.number
};
