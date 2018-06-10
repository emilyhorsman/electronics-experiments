import React, { Component } from "react";
import withUARTCharacteristic from "./withUARTCharacteristic";
import withGamepad from "./withGamepad";
import ControlPad from "./ControlPad";
import MessageTable from "./MessageTable";
import MessageInput from "./MessageInput";

import "./App.css";

class App extends Component {
  componentDidUpdate() {
    if (this.props.axes.length === 0 || !this.props.ready) {
      return;
    }

    this.props.sendFloatQuadString(this.props.axes);
  }

  render() {
    return (
      <div className="App">
        <div className="App__Header">
          <button
            onClick={this.props.requestDevice}
            disabled={this.props.ready || this.props.loading}
            className="App__RequestDevice"
          >
            {this.props.loading ? "Loading…" : "Request Device"}
          </button>

          {this.props.ready && (
            <span className="App__DeviceName">{this.props.deviceName}</span>
          )}
        </div>

        <ControlPad {...this.props} />
        <MessageInput {...this.props} />
        <MessageTable messages={this.props.messages} />
      </div>
    );
  }
}

export default withGamepad(withUARTCharacteristic(App));
