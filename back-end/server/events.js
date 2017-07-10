const assert = require('assert');
const log = require('../utils/log');
const {game} = require('../config');
const {genHealPack, genWeapon, genWall} = require('../utils/game');

// 各种和游戏有关的事件的处理器放这
exports.login = (server, player) => {
  assert(!server.game.started, 'you can only login before the game starts');
  server.playerPool.add(player);
  log.info(`player ${player.id} logged in`);
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id));
};

exports.logout = (server, player) => {
  server.playerPool.delete(player);
  log.info(`player ${player.id} logged out. ${server.playerPool.size} players remained`);
  if (server.game.started) {
    server.broadcast('logout', player.id);

    server.alivePlayer.delete(player);
    if (!server.playerPool.size) {
      log.info('Game over because there\'s no player');
      server.game.started = false;
    }
    if (server.alivePlayer.size === 1) {
      server.broadcast('gameover', {
        winner: [...server.alivePlayer.values()][0].id
      });
      server.game.started = false;
    }
  }
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id));
};

exports.name = (server, player, data) => {
  player.name = data.name;
};

exports.requireGameStart = (server, player) => {
  assert(!server.game.started, 'the game has already started');
  server.game.started = true;
  log.info('Game start');
  server.broadcast('gameStart');
  server.alivePlayer = new Set(server.playerPool);
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

exports.sync = (() => {
  let syncDataPool = new Map();
  return (server, player, data) => {
    syncDataPool.set(player.id, data);
    if (syncDataPool.size === server.playerPool.size) {
      // all sync-data received
      server.broadcast('sync', Array.from(syncDataPool).map(([key, value]) => {
        return {id: key, data: value};
      }));
      syncDataPool.clear();
    }
  };
})();

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
    server.broadcast(data.type, data, player);
    server.alivePlayer.delete(player);
    if (server.alivePlayer.size === 1) {
      // only 1 player left, game over
      server.broadcast('gameover', {
        winner: [...server.alivePlayer.values()][0].id
      });
      server.game.started = false;
    }
  }
};

const random = (min, max) => {
  return Math.floor(Math.random() * (max - min + 1)) + min;
};
