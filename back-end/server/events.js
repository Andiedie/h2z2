// 各种和游戏有关的事件的处理器放这

exports.raw = (server, player, message) => {
  console.log(`${player.id}: ${message}`);
};

exports.gameStart = (server, player, data) => {
  console.log('gameStart!');
};
