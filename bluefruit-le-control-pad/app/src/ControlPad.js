import React, { Component } from "react";

import "./ControlPad.css";

function getButton(event) {
  if (event.target !== document.body) {
    return null;
  }

  if (event.shiftKey || event.metaKey || event.ctrlKey) {
    return null;
  }

  const { keyCode } = event;
  switch (keyCode) {
    case 38:
      return 5;
    case 40:
      return 6;
    case 37:
      return 7;
    case 39:
      return 8;
    default:
      if (keyCode >= 49 && keyCode <= 52) {
        return keyCode - 48;
      }
      return null;
  }
}

function numToKey(key) {
  switch (key) {
    case 5:
      return "↑";
    case 6:
      return "↓";
    case 7:
      return "←";
    case 8:
      return "→";
    default:
      return "";
  }
}

class ControlPad extends Component {
  constructor(...args) {
    super(...args);

    this.isPressed = {};
  }

  componentDidMount() {
    document.body.addEventListener("keydown", this.onKeyDown);
    document.body.addEventListener("keyup", this.onKeyUp);
  }

  componentWillUnmount() {
    document.body.removeEventListener("keydown", this.onKeyDown);
    document.body.removeEventListener("keyup", this.onKeyUp);
  }

  onKeyDown = event => {
    const key = getButton(event);
    if (key) {
      event.preventDefault();
      this.onMouseDown(key);
    }
  };

  onKeyUp = event => {
    const key = getButton(event);
    if (key) {
      event.preventDefault();
      this.onMouseUp(key);
    }
  };

  onMouseDown = (key, _event) => {
    if (!this.props.ready) {
      return;
    }
    if (this.isPressed[key]) {
      return;
    }
    this.isPressed[key] = true;
    this.props.sendAsciiString(`!B${key}1`);
  };

  onMouseUp = (key, _event) => {
    if (!this.props.ready) {
      return;
    }
    if (!this.isPressed[key]) {
      return;
    }
    this.isPressed[key] = false;
    this.props.sendAsciiString(`!B${key}0`);
  };

  render() {
    return (
      <div className="ControlPad">
        <div className="ControlPad__DirectionalButtons">
          {[5, 6, 7, 8].map(key => (
            <button
              key={key}
              onMouseDown={this.onMouseDown.bind(null, key)}
              onMouseUp={this.onMouseUp.bind(null, key)}
              disabled={!this.props.ready}
              className={`ControlPad__Button ControlPad__Button--${key}`}
            >
              {numToKey(key)}
            </button>
          ))}
        </div>

        <div className="ControlPad__NumericButtons">
          {[1, 2, 3, 4].map(key => (
            <button
              key={key}
              onMouseDown={this.onMouseDown.bind(null, key)}
              onMouseUp={this.onMouseUp.bind(null, key)}
              disabled={!this.props.ready}
              className="ControlPad__Button"
            >
              {key}
            </button>
          ))}
        </div>
      </div>
    );
  }
}

export default ControlPad;
