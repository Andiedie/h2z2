const gen = require('crypto-random-string');
const {game} = require('../config');

exports.genHealPack = function * (num, hp) {
  while (num--) {
    yield {
      id: gen(8),
      posX: game.area.x * Math.random(),
      posY: game.area.y * Math.random(),
      hp
    };
  }
};
