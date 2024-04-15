const dgram = require('dgram');
const axios = require('axios');
const child = require("child_process");
require('dotenv').config();

const DISCORD_WEBHOOK = process.env.DISCORD_WEBHOOK;
if (!DISCORD_WEBHOOK) {
  throw new Error('No environment variable DISCORD_WEBHOOK found.');
}

const UDP_SERVER_ADDRESS = '192.168.6.1';
const UDP_SERVER_PORT = 7070;

const udpServer = dgram.createSocket('udp4');
udpServer.bind(UDP_SERVER_PORT, UDP_SERVER_ADDRESS);

const UDP_BBG_ADDRESS = '192.168.6.2';
const UDP_BBG_STREAMING_PORT = 1234;

let ffmpegProcess = startFFMpegProcess();
let frame = null;

// https://stackoverflow.com/questions/16245767/creating-a-blob-from-a-base64-string-in-javascript
const b64toBlob = (b64Data, contentType = '', sliceSize = 512) => {
  const byteCharacters = atob(b64Data);
  const byteArrays = [];

  for (let offset = 0; offset < byteCharacters.length; offset += sliceSize) {
    const slice = byteCharacters.slice(offset, offset + sliceSize);

    const byteNumbers = new Array(slice.length);
    for (let i = 0; i < slice.length; i++) {
      byteNumbers[i] = slice.charCodeAt(i);
    }

    const byteArray = new Uint8Array(byteNumbers);
    byteArrays.push(byteArray);
  }

  const blob = new Blob(byteArrays, { type: contentType });
  return blob;
};

// Send message to Discord through Webhook
const DISCORD_EMBED_COLOR = 5814783;

const sendDiscordMessage = (message, base64String = '') => {
  const image = base64String ? b64toBlob(base64String.replace(/^data:image\/[a-z]+;base64,/, ''), 'image/jpeg') : null;
  const data = {
    username: 'Security System',
    embeds: [
      {
        title: message,
        color: DISCORD_EMBED_COLOR,
        image: image
          ? {
            url: `attachment://image.jpeg`,
          }
          : null,
      },
    ],
  };

  // https://discord.com/developers/docs/reference#uploading-files
  var headers = new Headers();
  const formData = new FormData();
  if (image) formData.append('file', image, 'image.jpeg');
  formData.append('payload_json', JSON.stringify(data));
  headers.append('Content-Type', 'multipart/form-data');
  axios.post(DISCORD_WEBHOOK, formData, { headers: headers }).catch((error) => console.error(error));
};

udpServer.on('message', (msg) => {
  sendDiscordMessage(msg.toString(), getCurrentFrame());
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
});

function getCurrentFrame() {
  return frame ? "data:image/jpeg;base64," + frame : "";
}
