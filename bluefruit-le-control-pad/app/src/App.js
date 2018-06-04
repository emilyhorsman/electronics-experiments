import React, { Component } from "react";
import withUARTCharacteristic from "./withUARTCharacteristic";
import ControlPad from "./ControlPad";

import "./App.css";

class App extends Component {
  render() {
    return (
      <div className="App">
        <div className="App__Pairing">
          <button
            onClick={this.props.requestDevice}
            disabled={this.props.ready || this.props.loading}
          >
            {this.props.loading ? "Loading…" : "Request Device"}
          </button>
        </div>

        <ControlPad {...this.props} />
      </div>
    );
  }
}

export default withUARTCharacteristic(App);
