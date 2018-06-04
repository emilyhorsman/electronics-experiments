import React, { Component } from "react";
import "./App.css";

const uart_service_uuid = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const tx_characteristic_uuid = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

function char8StringToBufferWithChecksum(msg) {
  const buffer = new ArrayBuffer(msg.length + 1);
  const bufferView = new Uint8Array(buffer);

  let checksum = 0;
  for (let i = 0; i < msg.length; i++) {
    const c = msg.charCodeAt(i);
    bufferView[i] = c;
    checksum += c;
  }
  bufferView[msg.length] = 255 - checksum % 255;

  return buffer;
}

function getButtonFromKeyCode(keyCode) {
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
      return null;
  }
}

class App extends Component {
  constructor(...args) {
    super(...args);

    this.characteristic = null;
    this.isPressed = {};
    this.state = {
      ready: false
    };
  }

  componentDidMount() {
    window.addEventListener("keydown", this.onKeyDown, true);
    window.addEventListener("keyup", this.onKeyUp, true);
  }

  componentWillUnmount() {
    window.removeEventListener("keydown", this.onKeyDown);
    window.removeEventListener("keyup", this.onKeyUp);
  }

  discover = () => {
    navigator.bluetooth
      .requestDevice({
        acceptAllDevices: true,
        optionalServices: [uart_service_uuid]
      })
      .then(device => device.gatt.connect())
      .then(server => server.getPrimaryService(uart_service_uuid))
      .then(service => service.getCharacteristic(tx_characteristic_uuid))
      .then(characteristic => {
        this.characteristic = characteristic;
        this.setState({ ready: true });
      })
      .catch(error => {
        console.error("Error requesting Bluetooth device.", error);
      });
  };

  onKeyDown = event => {
    const key = getButtonFromKeyCode(event.keyCode);
    if (key) {
      event.preventDefault();
      this.onMouseDown(key);
    }
  };

  onKeyUp = event => {
    const key = getButtonFromKeyCode(event.keyCode);
    if (key) {
      event.preventDefault();
      this.onMouseUp(key);
    }
  };

  onMouseDown = (key, _event) => {
    if (!this.characteristic) {
      return;
    }
    if (this.isPressed[key]) {
      return;
    }
    this.isPressed[key] = true;
    const buffer = char8StringToBufferWithChecksum(`!B${key}1`);
    this.characteristic.writeValue(buffer);
  };

  onMouseUp = (key, _event) => {
    if (!this.characteristic) {
      return;
    }
    if (!this.isPressed[key]) {
      return;
    }
    this.isPressed[key] = false;
    const buffer = char8StringToBufferWithChecksum(`!B${key}0`);
    this.characteristic.writeValue(buffer);
  };

  render() {
    return (
      <div className="App">
        <button onClick={this.discover} disabled={this.state.ready}>
          Discover
        </button>

        <div className="App-buttons">
          {[1, 2, 3, 4].map(key => (
            <button
              key={key}
              onMouseDown={this.onMouseDown.bind(null, key)}
              onMouseUp={this.onMouseUp.bind(null, key)}
              disabled={!this.state.ready}
              className="App-button"
            >
              {key}
            </button>
          ))}
        </div>
      </div>
    );
  }
}

export default App;
