const WebSocket = require('ws');
const Player = require('./Player');
const events = require('./events');
const log = require('../utils/log');

module.exports = class Server {
  constructor () {
    // 等候玩家列表
    this.waitPool = new Set();
    // 游戏中玩家列表
    this.playerPool = null;
    // 存活玩家列表
    this.alivePool = null;
    // 同步数据
    this.syncPool = null;
    this.start = false;
  }

  listen (port) {
    this.server = new WebSocket.Server({port});
    this.server.on('connection', onConnection.bind(this));
  }

  broadcast (eventName, obj, except = null) {
    let list = eventName === 'waitList' ? this.waitPool : this.playerPool;
    for (const player of list) {
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
        try {
          player.sendError(err.message);
        } catch (err) {
          log.error(`${player.id}: ${err.message}`);
        }
      }
    });
  }
}
