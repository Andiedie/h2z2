// 各种和游戏有关的事件的处理器放这

exports.raw = (server, player, message) => {
  console.log(`${player.id}: ${message}`);
};

exports.login = (server, player) => {
  console.log(`player ${player.id} logged in`);
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id).join('|'));
};

exports.logout = (server, player) => {
  server.playerPool.delete(player);
  console.log(`player ${player.id} logged out`);
  server.broadcast('playerList', Array.from(server.playerPool).map(player => player.id).join('|'));
};
