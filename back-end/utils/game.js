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
      type: random(0, 4)
    };
  }
};

exports.randomColor = function () {
  return {
    r: random(190, 255),
    g: random(190, 255),
    b: random(190, 255)
  };
};

exports.genWall = function * (num) {
  while (num--) {
    yield {
      posX: game.area.x * Math.random(),
      posY: game.area.y * Math.random(),
      scaleX: Math.random() * 1.5 + 0.5,
      scaleY: Math.random() * 1.5 + 0.5,
      angle: Math.random() * 360
    };
  }
};

const random = (min, max) => {
  return Math.floor(Math.random() * (max - min + 1)) + min;
};
