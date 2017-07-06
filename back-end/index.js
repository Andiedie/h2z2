const config = require('./config');
const GameServer = require('./server');
const server = new GameServer();

server.listen(config.webSocket.port);
