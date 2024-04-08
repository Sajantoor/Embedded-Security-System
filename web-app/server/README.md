To run the server, run: `node index.js`

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
