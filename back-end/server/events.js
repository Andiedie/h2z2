const assert = require('assert');

// 各种和游戏有关的事件的处理器放这

exports.raw = (server, player, message) => {
  console.log(`${player.id}: ${message}`);
};

exports.login = (server, player) => {
  assert(!server.game.started, 'you can only login before the game starts');
  server.playerPool.add(player);
  console.log(`player ${player.id} logged in`);
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id).join('|'));
};

exports.logout = (server, player) => {
  server.playerPool.delete(player);
  console.log(`player ${player.id} logged out`);
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id).join('|'));
};

exports.requireGameStart = (server, player) => {
  assert(!server.game.started, 'the game has already started');
  // server.game.started = true;
  server.broadcast('gameStart');
  let players;
  server.broadcastMap('initData', (server, player) => {
    if (!players) players = Array.from(server.playerPool).map(player => player.id);
    return {
      players,
      selfId: player.id
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
