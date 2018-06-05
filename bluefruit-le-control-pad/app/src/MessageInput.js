import React, { Component } from "react";

class MessageInput extends Component {
  constructor(...args) {
    super(...args);

    this.state = {
      input: ""
    };
  }

  onSubmit = event => {
    event.preventDefault();
    this.props.sendAsciiString(this.state.input);
    this.setState({
      input: ""
    });
  };

  onChange = event => {
    this.setState({
      input: event.target.value
    });
  };

  render() {
    return (
      <form className="MessageInput" onSubmit={this.onSubmit}>
        <input
          type="text"
          value={this.state.input}
          onChange={this.onChange}
          disabled={!this.props.ready}
        />
        <button type="submit" disabled={!this.props.ready}>
          Send
        </button>
      </form>
    );
  }
}

export default MessageInput;
