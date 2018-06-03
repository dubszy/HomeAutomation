import React from 'react'
import PropTypes from 'prop-types'

import classes from './Row.css'

export default class Row extends React.Component {

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
      minHeight,
      maxHeight,
      minWidth,
      maxWidth,
      zIndex,
      ...rest
    } = props;

    classNames.push('row');

    if (className) {
      classNames.push(className);
    }

    if (typeof flex !== 'undefined') {
      styles.flex = flex;
    }

    if (typeof order !== 'undefined') {
      styles.order = order;
    }

    if (typeof minHeight !== 'undefined') {
      styles.minHeight = minHeight;
    }

    if (typeof maxHeight !== 'undefined') {
      styles.maxHeight = maxHeight;
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
    const processedProps = Row.propcessor(this.props);

    return (
      <div {...processedProps}>
        {this.props.children}
      </div>
    )
  }
};

Row.propTypes = {
  className: PropTypes.string,
  order: PropTypes.number,
  minHeight: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  maxHeight: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  minWidth: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  maxWidth: PropTypes.oneOfType([PropTypes.number, PropTypes.string]),
  zIndex: PropTypes.number

}