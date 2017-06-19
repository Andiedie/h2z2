const assert = require('assert');
const {EventEmitter} = require('events');
const WebSocket = require('ws');
const {webSocket: config} = require('../config');

const Player = require('./Player');
const events = require('./events');

module.exports = class Server extends EventEmitter {
  constructor () {
    super();
    this.playerPool = new Set();
    this.game = {
      started: false
    };

    this.server = new WebSocket.Server({
      port: config.port
    });

    this.server.on('connection', this.onConnection.bind(this));
  }

  onConnection (ws) {
    const player = new Player(ws);
    for (const [eventName, handler] of Object.entries(events)) {
      player.on(eventName, (...args) => {
        try {
          handler(this, ...args);
        } catch (err) {
          player.sendError(err.stack);
        }
      });
    }
  }

  broadcast (eventName, obj, except = null) {
    for (const player of this.playerPool) {
      if (player === except) continue;
      player.sendEvent(eventName, obj);
    }
  }

  // costy, be careful
  broadcastMap (eventName, fn, except = null) {
    const datas = Array.from(this.playerPool).map(player => fn(this, player));
    let i = 0;
    for (const player of this.playerPool) {
      if (player === except) continue;
      player.sendEvent(eventName, datas[i]);
      i++;
    }
  }
};
