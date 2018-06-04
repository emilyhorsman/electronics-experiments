(function() {
  const uart_service_uuid = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
  const tx_characteristic_uuid = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

  let remote_characteristic = null;

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

  function discover() {
    navigator.bluetooth
      .requestDevice({
        acceptAllDevices: true,
        optionalServices: [uart_service_uuid]
      })
      .then(device => device.gatt.connect())
      .then(server => server.getPrimaryService(uart_service_uuid))
      .then(service => service.getCharacteristic(tx_characteristic_uuid))
      .then(characteristic => (remote_characteristic = characteristic))
      .catch(error => {
        console.log("Error requesting Bluetooth device.", error);
      });
  }

  function write(event) {
    if (!remote_characteristic) {
      return;
    }

    const buffer = char8StringToBufferWithChecksum(
      `!B${event.target.dataset.key}1`
    );
    remote_characteristic.writeValue(buffer);
  }

  window.addEventListener("DOMContentLoaded", () => {
    document
      .querySelector(".js-bluetooth-discovery")
      .addEventListener("click", discover);

    document.querySelectorAll(".js-button").forEach(btn => {
      btn.addEventListener("click", write);
    });
  });
})();
