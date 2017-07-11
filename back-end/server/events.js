const assert = require('assert');
const log = require('../utils/log');
const {game} = require('../config');
const {genHealPack, genWeapon, genWall} = require('../utils/game');

// 各种和游戏有关的事件的处理器放这
exports.login = (server, player) => {
  server.waitPool.add(player);
  log.info(`player ${player.id} enter waiting hall. ${server.waitPool.size} waiting`);
};

exports.logout = (server, player) => {
  if (server.waitPool.has(player)) {
    log.info(`player ${player.id} leave waiting hall. ${server.waitPool.size} waiting`);
    server.waitPool.delete(player);
    server.broadcast('waitList', Array.from(server.waitPool).map(player => player.name || 'no name'));
  } else {
    log.info(`player ${player.id} leave game. ${server.playerPool.size} playing`);
    server.playerPool.delete(player);
    server.alivePool.delete(player);
    server.broadcast('logout', player.id);
    if (!server.playerPool.size) {
      log.info('Game over because there\'s no player');
      server.start = false;
    } else if (server.alivePool.size === 1) {
      const winner = [...server.alivePool.values()][0].id;
      log.info(`Game over with winner ${winner}`);
      server.broadcast('gameover', {winner});
      server.playerPool.forEach(server.waitPool.add.bind(server.waitPool));
      server.broadcast('waitList', Array.from(server.waitPool).map(player => player.name || 'no name'));
      server.start = false;
    }
  }
};

exports.name = (server, player, data) => {
  player.name = data.name;
  server.broadcast('waitList', Array.from(server.waitPool).map(player => player.name || 'no name'));
};

exports.requireGameStart = (server, player) => {
  assert(!server.start, 'Please wait for the end of the last game');
  server.playerPool = new Set(server.waitPool);
  server.alivePool = new Set(server.playerPool);
  server.syncPool = new Map();
  server.waitPool.clear();
  server.start = true;
  log.info(`Game start with ${server.playerPool.size} players`);
  server.broadcast('gameStart');
  let players = Array.from(server.playerPool).map(player => {
    return {
      id: player.id,
      name: player.name || '',
      color: {
        r: random(190, 255),
        g: random(190, 255),
        b: random(190, 255)
      }
    };
  });
  let healPacks = [...genHealPack(game.healPack.ratio * server.playerPool.size, game.healPack.hp)];
  let weapons = [...genWeapon(game.weapon.ratio * server.playerPool.size)];
  let walls = [...genWall(game.wall.num)];
  server.broadcastMap('initData', (server, player) => {
    return {
      players,
      healPacks,
      weapons,
      walls,
      selfId: player.id,
      selfPos: {x: Math.random() * game.area.x, y: Math.random() * game.area.y}
    };
  });
};

exports.sync = (server, player, data) => {
  server.syncPool.set(player.id, data);
  if (server.syncPool.size === server.playerPool.size) {
    // all sync-data received
    server.broadcast('sync', Array.from(server.syncPool).map(([key, value]) => {
      return {id: key, data: value};
    }));
    server.syncPool.clear();
  }
};

exports.broadcast = (server, player, data) => {
  data.from = player.id; // add broadcaster info, may be used later
  assert(data.type, 'attribute [type] required');
  if (broadcastHook[data.type]) {
    broadcastHook[data.type](server, player, data);
  } else {
    server.broadcast(data.type, data, player);
  }
};

const broadcastHook = {
  dead (server, player, data) {
    server.broadcast('dead', data, player);
    server.alivePool.delete(player);
    if (server.alivePool.size === 1) {
      const winner = [...server.alivePool.values()][0].id;
      log.info(`Game over with winner ${winner}`);
      server.broadcast('gameover', {winner});
      server.playerPool.forEach(server.waitPool.add.bind(server.waitPool));
      server.broadcast('waitList', Array.from(server.waitPool).map(player => player.name || 'no name'));
      server.start = false;
    }
  }
};

const random = (min, max) => {
  return Math.floor(Math.random() * (max - min + 1)) + min;
};
