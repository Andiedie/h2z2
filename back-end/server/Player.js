const {EventEmitter} = require('events');
const assert = require('assert');
const gen = require('crypto-random-string');
const log = require('../utils/log');

module.exports = class Player extends EventEmitter {
  constructor (webSocket) {
    super();
    this.ws = webSocket;
    this.id = gen(8);

    webSocket.on('message', message => {
      try {
        const obj = JSON.parse(message);
        const event = obj.event;
        assert(event && this.listenerCount(event), `Unexpected event: ${event}`);
        this.emit(event, obj.data);
      } catch (err) {
        log.error(err.stack);
        this.sendError(err.message);
      }
    });

    webSocket.on('close', () => this.emit('logout'));

    // 在constructor后才绑定的事件
    setImmediate(() => this.emit('login'));
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
