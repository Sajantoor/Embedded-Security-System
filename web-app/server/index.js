const { Server } = require('socket.io');
const dgram = require('dgram');
const axios = require('axios');
require('dotenv').config();
const child = require("child_process");

const DISCORD_WEBHOOK = process.env.DISCORD_WEBHOOK;
if (!DISCORD_WEBHOOK) {
  throw new Error('No environment variable DISCORD_WEBHOOK found.');
}

const webSocketServer = new Server({
  cors: {
    origin: 'http://localhost:3000',
  },
});

const port = 4000;
webSocketServer.listen(port);

const DISCORD_EMBED_COLOR = 5814783;

const UDP_SERVER_ADDRESS = '127.0.0.1';
const UDP_SERVER_PORT = 7070;

const UDP_BBG_ADDRESS = "192.168.6.1";
const UDP_BBG_PORT = 1234;

const udpServer = dgram.createSocket('udp4');
udpServer.bind(UDP_SERVER_PORT, UDP_SERVER_ADDRESS);

let ffmpegProcess = startFFMpegProcess();
let frame = null;
let currentClientSocket = null;

// Send message to Discord through Webhook
const sendDiscordMessage = (message) => {
  axios
    .post(DISCORD_WEBHOOK, {
      username: 'Security System',
      embeds: [
        {
          title: message,
          color: DISCORD_EMBED_COLOR,
        },
      ],
    })
    .catch((error) => {
      console.error('Error:', error);
    });
};

webSocketServer.on('connection', (socket) => {
    console.log("Connected");
    currentClientSocket = socket;

  udpServer.on('message', (msg) => {
    console.log('Received message from UDP server: ', msg.toString());
    socket.emit('fmessage', msg.toString());
    });

    socket.on("connect", (client) => {
        console.log("user connected");
        client.emit("message", "Connected to server");
  });

    socket.on("disconnect", () => {
        currentClientSocket = null;
        console.log("user disconnected");
    });

  socket.on('message', (message) => {
    console.log('Received message: ', message);
    udpServer.send(message, 0, message.length, UDP_BBG_PORT, UDP_BBG_ADDRESS);
    sendDiscordMessage(message);
  });
});

function startFFMpegProcess() {
    console.log("ffmpeg process started");

    const ffmpegArgs = [
        "-re",
        "-y",
        "-i",
        `udp://${UDP_BBG_ADDRESS}:${UDP_BBG_PORT}`,
        "-preset",
        "ultrafast",
        "-f",
        "mjpeg",
        "pipe:1"
    ];

    return child.spawn("ffmpeg", ffmpegArgs);
}


ffmpegProcess.on("connect", function () {
    console.log("ffmpeg connected");
});

ffmpegProcess.on("error", function (err) {
    console.log(err);
    throw err;
});

ffmpegProcess.on("close", function (code) {
    console.log("ffmpeg exited with code " + code);
    ffmpegProcess = null;
    setTimeout(function(){
        ffmpegProcess = startFFMpegProcess();
    }, 50);
});

ffmpegProcess.stderr.on("data", function (data) {
    // Don't remove this
    // Child Process hangs when stderr exceed certain memory
});

ffmpegProcess.stdout.on("data", function (data) {
    frame = Buffer.from(data).toString("base64"); //convert raw data to string

    if (currentClientSocket) {
        currentClientSocket.emit("canvas", frame);
    }

});

function getCurrentFrame(){
  return "data:image/jpeg;base64," + frame;
}