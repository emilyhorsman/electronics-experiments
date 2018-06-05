import React, { Component } from "react";
import { Column, Table } from "react-virtualized";

import "react-virtualized/styles.css";
import "./MessageTable.css";

class MessageTable extends Component {
  render() {
    return (
      <Table
        className="MessageTable"
        width={600}
        height={300}
        rowHeight={18}
        rowCount={this.props.messages.length}
        rowGetter={({ index }) => this.props.messages[index]}
      >
        <Column
          dataKey="timestamp"
          width={150}
          cellRendered={({ cellData }) => cellData.toString()}
        />
        <Column dataKey="type" width={50} />
        <Column dataKey="message" width={400} />
      </Table>
    );
  }
}

export default MessageTable;
