import React, { Component } from "react";
import withUARTCharacteristic from "./withUARTCharacteristic";
import ControlPad from "./ControlPad";

import "./App.css";

class App extends Component {
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
      </div>
    );
  }
}

export default withUARTCharacteristic(App);
