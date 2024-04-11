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
console.log(`WebSocket server listening on port ${port}`);

const DISCORD_EMBED_COLOR = 5814783;

const UDP_SERVER_ADDRESS = '192.168.6.1';
const UDP_SERVER_PORT = 7070;

const UDP_BBG_ADDRESS = '192.168.6.2';
const UDP_BBG_PORT = 12345;
const UDP_BBG_STREAMING_PORT = 1234;

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
    handleMessage(msg.toString(), socket);
    socket.emit('message', msg.toString());
  });

  socket.on("connect", () => {
    console.log("user connected");
  });

  socket.on("disconnect", () => {
    currentClientSocket = null;
    console.log("user disconnected");
  });

  socket.on('message', (message) => {
    console.log('Received message: ', message);
    udpServer.send(message, UDP_BBG_PORT, UDP_BBG_ADDRESS);
  });
});

function startFFMpegProcess() {
  console.log("ffmpeg process started");

  const ffmpegArgs = [
    "-re",
    "-y",
    "-i",
    `udp://${UDP_BBG_ADDRESS}:${UDP_BBG_STREAMING_PORT}?overrun_nonfatal=1`,
    "-preset",
    "ultrafast",
    "-f",
    "mjpeg",
    "-tune",
    "zerolatency",
    "-omit_video_pes_length",
    "1",
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
  setTimeout(function () {
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

function getCurrentFrame() {
  return "data:image/jpeg;base64," + frame;
}

const COMMANDS = {
  DOOR_OPEN: 'doorOpen',
  DOOR_CLOSED: 'doorClosed',
  FAILED_PASSWORD: 'failedPassword',
  MOTION_DETECTED: 'motionDetected',
  PASSWORD_CHANGED: 'passwordChanged',
  PASSWORD_SET: 'passwordSet',
  PASSWORD_CHANGE_FAILED: 'passwordChangeFailed',
  DISPLAY_MESSAGE_SET: 'displayMessageSet',
  DISPLAY_MESSAGE_FAILED: 'displayMessageFailed'
};

function handleMessage(message, socket) {
  const tokens = message.split(' ');
  const command = tokens[0];
  const epochTime = parseInt(tokens[1]);

  const date = new Date(0);
  date.setUTCSeconds(epochTime);
  const timestamp = date.toLocaleString("en-US", { timeZone: "America/Vancouver" });

  let messageSent = '';

  switch (command) {
    case COMMANDS.DOOR_OPEN:
      message = 'Door Opened';
      break;
    case COMMANDS.DOOR_CLOSED:
      message = 'Door Closed';
      break;
    case COMMANDS.FAILED_PASSWORD:
      messageSent = tokens.slice(2).join(' ');
      message = `Failed Password: ${messageSent}`;
      break;
    case COMMANDS.PASSWORD_CHANGED:
      message = 'Password Changed';
      break;
    case COMMANDS.PASSWORD_SET:
      message = 'Password Set';
      break;
    case COMMANDS.PASSWORD_CHANGE_FAILED:
      message = 'Password Change Failed';
      break;
    case COMMANDS.DISPLAY_MESSAGE_SET:
      message = 'Display Message Set'
      break;
    case COMMANDS.MOTION_DETECTED:
      message = 'Motion Detected'
      break;
    case COMMANDS.DISPLAY_MESSAGE_FAILED:
      messageSent = tokens.slice(2).join(' ');
      message += `Display Message Failed: ${messageSent}`;
      break;
    default:
      console.error(`Unknown command: ${command} and message: ${message}`);
      return;
  }

  const discordMessage = `**${timestamp}**: ${message}`;
  sendDiscordMessage(discordMessage);

  if (socket) {
    socket.emit('event', {
      message,
      epochTime,
      image: getCurrentFrame()
    });
  }
}
