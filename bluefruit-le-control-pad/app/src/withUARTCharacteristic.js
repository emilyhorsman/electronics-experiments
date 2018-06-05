import React, { Component } from "react";

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

function withUARTCharacteristic(WrappedComponent) {
  return class extends Component {
    constructor(...args) {
      super(...args);

      this.characteristic = null;
      this.isPressed = {};
      this.state = {
        ready: false,
        loading: false,
        deviceName: null
      };
    }

    onDisconnect = () => {
      this.setState({
        ready: false,
        deviceName: null
      });
    };

    requestDevice = async () => {
      this.setState({
        loading: true
      });

      return navigator.bluetooth
        .requestDevice({
          filters: [{ services: [uart_service_uuid] }],
          optionalServices: [uart_service_uuid]
        })
        .then(device => {
          device.addEventListener("gattserverdisconnected", this.onDisconnect);
          return device.gatt.connect();
        })
        .then(server => server.getPrimaryService(uart_service_uuid))
        .then(service => service.getCharacteristic(tx_characteristic_uuid))
        .then(characteristic => {
          this.characteristic = characteristic;
          this.setState({
            ready: true,
            loading: false,
            deviceName: characteristic.service.device.name
          });
        })
        .catch(error => {
          console.error("Error requesting Bluetooth device.", error);
          this.setState({
            ready: false,
            loading: false
          });
        });
    };

    sendAsciiString = message => {
      const buffer = char8StringToBufferWithChecksum(message);
      this.characteristic.writeValue(buffer);
    };

    render() {
      return (
        <WrappedComponent
          ready={this.state.ready}
          loading={this.state.loading}
          deviceName={this.state.deviceName}
          sendAsciiString={this.sendAsciiString}
          requestDevice={this.requestDevice}
        />
      );
    }
  };
}

export default withUARTCharacteristic;