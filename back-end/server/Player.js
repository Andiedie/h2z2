const {EventEmitter} = require('events');
const assert = require('assert');
const gen = require('crypto-random-string');

module.exports = class Player extends EventEmitter {
  constructor (webSocket) {
    super();
    this.ws = webSocket;
    this.id = gen(8);

    console.log(`new player ${this.id}`);

    webSocket.on('message', message => {
      try {
        const obj = JSON.parse(message);
        const event = obj.event || 'message';
        // assert(obj.hasOwnProperty('data'), 'no prop [data]');
        if (!this.emit(event, this, obj.data)) {
          console.error(`no such event handler: ${event}`);
        }
      } catch (err) {
        // raw data
        this.emit('raw', this, message);
      }
    });

    webSocket.on('close', () => {
      console.log(`player ${this.id} disconnected`);
      this.emit('logout', this);
    });

    // 在constructor后才绑定的事件
    setImmediate(() => {
      this.emit('login', this);
    });
  }

  sendEvent (eventName, data) {
    this.ws.send(JSON.stringify({
      event: eventName,
      data
    }));
  }

  sendError (message) {
    this.sendEvent('error', message);
  }
};
