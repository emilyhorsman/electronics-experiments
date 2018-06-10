import React, { Component } from "react";

export default function withGamepad(WrappedComponent) {
  return class extends Component {
    constructor(...args) {
      super(...args);

      this.gamePad = null;
      this.state = {
        axes: []
      };
    }

    componentDidMount() {
      window.addEventListener("gamepadconnected", this.onGamePadConnected);
      window.addEventListener(
        "gamepaddisconnected",
        this.onGamePadDisconnected
      );
    }

    componentWillUnmount() {
      window.removeEventListener("gamepadconnected", this.onGamePadConnected);
      window.removeEventListener(
        "gamepaddisconnected",
        this.onGamePadDisconnected
      );
    }

    onGamePadConnected = event => {
      if (this.gamePad) {
        console.warn("Gamepad already connected. Ignoring new connection.");
        return;
      }

      this.gamePad = event.gamepad;
      requestAnimationFrame(this.update);
    };

    onGamePadDisconnected = event => {
      if (event.gamepad.id === this.gamePad.id) {
        this.gamePad = null;
      }
    };

    update = () => {
      if (!this.gamePad) {
        return null;
      }

      const { axes } = navigator.getGamepads()[this.gamePad.index];
      this.setState({ axes });
      requestAnimationFrame(this.update);
    };

    render() {
      return <WrappedComponent axes={this.state.axes} {...this.props} />;
    }
  };
}
