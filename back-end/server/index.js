const WebSocket = require('ws');
const Player = require('./Player');
const events = require('./events');
const log = require('../utils/log');

module.exports = class Server {
  constructor () {
    this.playerPool = new Set();
    this.alivePlayer = null;
    this.game = {
      started: false
    };
  }

  listen (port) {
    this.server = new WebSocket.Server({port});
    this.server.on('connection', onConnection.bind(this));
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

function onConnection (ws) {
  const player = new Player(ws);
  for (const [eventName, handler] of Object.entries(events)) {
    player.on(eventName, (...args) => {
      try {
        handler(this, player, ...args);
      } catch (err) {
        log.error(err.stack);
        player.sendError(err.message);
      }
    });
  }
}
