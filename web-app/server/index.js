const { Server } = require('socket.io');
const dgram = require('dgram');
const axios = require('axios');
require('dotenv').config();

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

const UDP_BBG_ADDRESS = '192.168.6.2';
const UDP_BBG_PORT = 7070;

const udpServer = dgram.createSocket('udp4');
udpServer.bind(UDP_SERVER_PORT, UDP_SERVER_ADDRESS);

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
  udpServer.on('message', (msg) => {
    console.log('Received message from UDP server: ', msg.toString());
    handleMessage(msg.toString(), socket);
  });

  socket.on('disconnect', () => {
    console.log('user disconnected');
  });

  socket.on('message', (message) => {
    console.log('Received message: ', message);
    udpServer.send(message, 0, message.length, UDP_BBG_PORT, UDP_BBG_ADDRESS);
  });
});

const COMMANDS = {
  DOOR_OPEN: 'doorOpen',
  DOOR_CLOSE: 'doorClose',
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
  const epochTime = tokens[1];

  const date = new Date();
  date.setUTCSeconds(epochTime);
  const timestamp = date.toLocaleString();

  let message;
  let messageSent = '';

  switch (command) {
    case COMMANDS.DOOR_OPEN:
      message = 'Door Opened';
      break;
    case COMMANDS.DOOR_CLOSE:
      message = 'Door Closed';
      break;
    case COMMANDS.FAILED_PASSWORD:
      messageSent = tokens[2];
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
      messageSent = tokens[2];
      message += `Display Message Failed: ${messageSent}`;
      break;
    default:
      console.error(`Unknown command: ${command} and message: ${message}`);
      return;
  }

  const discordMessage = `**${timestamp}**: ${message}`;
  sendDiscordMessage(discordMessage);

  socket.emit('event', {
    message,
    epochTime,
  });
}