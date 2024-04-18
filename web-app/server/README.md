# Security System Server

Ensure you have created a `.env` file with the content in the `.env.template` file.
To get a Discord webhook visit the following link: https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks

Start the server with the following commands:

```
npm install
npm start
```

You can test the server by sending a message to the WebSocket server using the following code in a separate file (e.g., client.js):

```
const io = require('socket.io-client');

const socket = io('http://localhost:4000', {
	transports: ['websocket'],
});

socket.on('connect', () => {
	socket.emit('message', 'Hello from WebSocket client!');
});
```

Resources:

https://discord.com/developers/docs/resources/webhook
