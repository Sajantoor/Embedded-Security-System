const { Server } = require("socket.io");
const dgram = require("dgram");

const webSocketServer = new Server({
    cors: {
        origin: "http://localhost:3000",
    },
});

const port = 4000;
webSocketServer.listen(port);

const UDP_SERVER_ADDRESS = "127.0.0.1";
const UDP_SERVER_PORT = 7070;

const UDP_BBG_ADDRESS = "192.168.6.2";
const UDP_BBG_PORT = 7070;

const udpServer = dgram.createSocket("udp4");
udpServer.bind(UDP_SERVER_PORT, UDP_SERVER_ADDRESS);

webSocketServer.on("connection", (socket) => {
    udpServer.on("message", (msg) => {
        console.log("Received message from UDP server: ", msg.toString());
        socket.emit("message", msg.toString());
    });

    socket.on("disconnect", () => {
        console.log("user disconnected");
    });

    socket.on("message", (message) => {
        console.log("Received message: ", message);
        udpServer.send(message, 0, message.length, UDP_BBG_PORT, UDP_BBG_ADDRESS);
    });
});
