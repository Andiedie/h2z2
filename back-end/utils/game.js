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

exports.genWeapon = function * (num) {
  while (num--) {
    yield {
      id: gen(8),
      posX: game.area.x * Math.random(),
      posY: game.area.y * Math.random(),
      type: randomType()
    };
  }
};

function randomType () {
  const random = Math.random();
  if (random < 0.4) return 0;
  if (random < 0.55) return 1;
  if (random < 0.7) return 2;
  if (random < 0.85) return 3;
  return 4;
}
