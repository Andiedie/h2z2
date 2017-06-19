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
  server.game.started = true;
  server.broadcast('gameStart');
};
