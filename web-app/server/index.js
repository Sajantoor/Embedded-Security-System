const { Server } = require('socket.io');
const dgram = require('dgram');

const webSocketServer = new Server({
    cors: {
        origin: "http://localhost:3000"
    }
});

const port = 4000;

webSocketServer.listen(port);

const UDP_SERVER_ADDRESS = "127.0.0.1";
const UDP_SERVER_PORT = 7070;

const udpServer = dgram.createSocket('udp4');
udpServer.bind(UDP_SERVER_PORT, UDP_SERVER_ADDRESS);

webSocketServer.on('connection', (socket) => {
    udpServer.on('message', (msg) => {
        socket.emit('message', msg.toString());
    });

    socket.on('disconnect', () => {
        console.log('user disconnected');
    });
});




