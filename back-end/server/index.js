const assert = require('assert');
const {EventEmitter} = require('events');
const WebSocket = require('ws');
const {webSocket: config} = require('../config');

const events = require('./events');

module.exports = class Server extends EventEmitter {
  constructor () {
    super();
    this.playerPool = new Set();

    this.server = new WebSocket.Server({
      port: config.port
    });

    this.server.on('connection', this.onConnection.bind(this));

    for (const [event, handler] of Object.entries(events)) {
      this.on(event, handler.bind(this));
    }
  }

  onConnection (ws) {
    console.log('new player!');
    ws.send('bello!');
    this.playerPool.add(ws);

    ws.on('message', message => {
      console.log('message');
      console.log(message);
      const obj = JSON.parse(message);
      const event = obj.event || 'message';
      this.emit(event, obj.data);
    });
  }

  broadcast (eventName, obj) {
    for (const socket of this.playerPool) {
      emit(socket, eventName, obj);
    }
  }
};

function emit (socket, eventName, obj) {
  socket.send(JSON.stringify({
    event: eventName,
    data: obj
  }));
}
