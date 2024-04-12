import { useState, useEffect, useRef } from 'react';
import io from 'socket.io-client';
import './index.css';
import {
  NumberInput,
  NumberInputField,
  NumberIncrementStepper,
  NumberDecrementStepper,
  NumberInputStepper,
  Text,
  Image,
  Box,
  Switch,
  Input,
  Button,
  Flex,
  Accordion,
  AccordionItem,
  AccordionButton,
  AccordionPanel,
  AccordionIcon,
} from '@chakra-ui/react';

const DISPLAY_COMMAND = 'setDisplayMessage';
const LOCK_COMMAND = 'lock';
const UNLOCK_COMMAND = 'unlock';
const CHANGE_PASSWORD_COMMAND = 'changePassword';
const SHUTDOWN = 'shutdown';

export default function Home() {
  const [systemStatus, setSystemStatus] = useState('shutdown');
  const [lockStatus, setLockStatus] = useState('locked');
  const [lastUpdated, setLastUpdated] = useState(new Date());
  const [displayMessage, setDisplayMessage] = useState('');
  const [currentMessage, setCurrentMessage] = useState('');
  const [password, setPassword] = useState('');
  const [newPassword, setNewPassword] = useState('');
  const [confirmPassword, setConfirmPassword] = useState('');
  const [displayTimeout, setDisplayTimeout] = useState(0);
  const [socket, setSocket] = useState(null);
  const [uptime, setUptime] = useState(0);
  const [events, setEvents] = useState([]);
  const [errors, setErrors] = useState([]);
  const canvasRef = useRef(null);
  let timeout = null;

  useEffect(() => {
    const url = 'http://localhost:4000';
    const socket = io(url);
    setSocket(socket);

    // Listen for connection event
    socket.on("connect", (socket) => {
      console.log("Connected to server");
    });

    socket.on('event', (event) => {
      console.log(event);
      const date = new Date(0);
      date.setUTCSeconds(event.epochTime);
      event.timestamp = date;

      if (!event.image) {
        event.image = '/loading.jpg';
      }

      // if the event message contains 'door open' or 'door close', setDoorStatus
      if (event.message.includes('Door Opened')) {
        setLockStatus('unlocked');
      } else if (event.message.includes('Door Closed')) {
        setLockStatus('locked');
      } else if (event.message.includes('Display Message Set')) {
        return;
      }

      setEvents((prevEvents) => [event, ...prevEvents]);
    });

    socket.on('heartbeat', (heartbeat) => {
      // message has the format [heartbeat] [timestamp] [uptime (in seconds)] [currentMessage]
      clearTimeout(timeout);


      const tokens = heartbeat.split(' ');
      const epochTime = parseInt(tokens[1]);
      const uptime = parseInt(tokens[2]);
      const currentMessage = tokens.slice(3).join(' ');

      const date = new Date(0);
      date.setUTCSeconds(epochTime);
      setLastUpdated(date);
      setUptime(uptime);
      setCurrentMessage(currentMessage);
      setSystemStatus('Online');
      setErrors([]);

      timeout = setTimeout(() => {
        systemDown();
      }, 3000);
    });

    socket.on("canvas", (data) => {
      drawCanvas(data);
    });

    // Clean up the socket connection on unmount
    return () => {
      socket.disconnect();
    };
  }, []);

  function systemDown() {
    setSystemStatus('Offline');
    setErrors(['System is down!']);
    setUptime('N/A');
  }

  function drawCanvas(data) {
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    const image = new window.Image();
    image.src = "data:image/jpeg;base64," + data;
    image.onload = function () {
      context.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas
      context.height = image.height;
      context.width = image.width;
      context.drawImage(image, 0, 0, context.width, context.height);
    };
  }

  function sendMessageToServer(message) {
    if (socket) {
      socket.emit('message', message);
      console.log('Sent message to server: ', message);
    }
  }

  function handleLockToggle() {
    const newLockStatus = lockStatus === 'locked' ? 'unlocked' : 'locked';

    if (newLockStatus === 'locked') {
      sendMessageToServer(LOCK_COMMAND);
    } else {
      sendMessageToServer(UNLOCK_COMMAND);
    }
  }

  function handleChangeMessage() {
    if (!displayMessage) {
      return;
    }

    let timeoutValue = 0;
    if (displayTimeout) {
      timeoutValue = parseInt(displayTimeout);
    }

    if (timeoutValue > 0) {
      sendMessageToServer(`${DISPLAY_COMMAND} ${timeoutValue} ${displayMessage}`);
    } else {
      sendMessageToServer(`${DISPLAY_COMMAND} 0 ${displayMessage}`);
    }

    setDisplayTimeout(0);
    setDisplayMessage('');
  }

  function handlePasswordChange() {
    if (!password || !newPassword || !confirmPassword) {
      alert('Please fill all fields');
      return;
    }

    if (!/^[1-8]{4}$/.test(password) || !/^[1-8]{4}$/.test(newPassword)) {
      alert('Password should be 4 digits long and only contain digits 1 - 8');
      return;
    }

    if (newPassword !== confirmPassword) {
      alert('Passwords do not match');
      return;
    }

    sendMessageToServer(`${CHANGE_PASSWORD_COMMAND} ${password} ${newPassword}`);
    setPassword('');
    setNewPassword('');
    setConfirmPassword('');
  }

  function handleShutdown() {
    sendMessageToServer(SHUTDOWN);
  }

  function showUptime() {
    const seconds = uptime % 60;
    const minutes = Math.floor(uptime / 60) % 60;
    const hours = Math.floor(uptime / 3600) % 24;
    const days = Math.floor(uptime / 86400);

    // if fields are zero don't show them
    if (days === 0 && hours === 0 && minutes === 0) {
      return `${seconds}s`;
    } else if (days === 0 && hours === 0) {
      return `${minutes}m ${seconds}s`;
    } else if (days === 0) {
      return `${hours}h ${minutes}m ${seconds}s`;
    }

    return `${days}d ${hours}h ${minutes}m ${seconds}s`;
  }

  return (
    <Box p="10">
      <Flex gap="10" className="flex-col lg:flex-row">
        <Flex flexDirection="column" className="capitalize lg:w-1/2 xl:w-1/3" gap="4">
          {/* <Image src={cameraFeed || '/loading.jpg'} alt="Security Camera" className="" /> */}
          <canvas ref={canvasRef} width="720" height="480"></canvas>
          <Box>
            <Text className="text-3xl font-bold">Show Message</Text>
            <Text className="pb-2 normal-case">Display a message with a timeout. 0 is no timeout.</Text>
            <Flex gap={2}>
              <Input
                placeholder="Enter message"
                value={displayMessage}
                onChange={(e) => setDisplayMessage(e.target.value)}
                className="w-1/2"
              />
              <NumberInput min={0} value={displayTimeout} onChange={setDisplayTimeout} className="w-1/2">
                <NumberInputField />
                <NumberInputStepper>
                  <NumberIncrementStepper />
                  <NumberDecrementStepper />
                </NumberInputStepper>
              </NumberInput>
              <Button onClick={handleChangeMessage} className="shrink-0">
                Send
              </Button>
            </Flex>
          </Box>

          <Box>
            <Text className="text-3xl font-bold pb-2">Change Password</Text>
            <Flex gap={2} flexDirection="column">
              <Input
                placeholder="Enter current password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                type="password"
                pattern="[0-8]*"
                inputmode="numeric"
              />
              <Flex gap={2}>
                <Input
                  placeholder="Enter new password"
                  type="password"
                  pattern="[0-8]*"
                  inputmode="numeric"
                  value={newPassword}
                  onChange={(e) => setNewPassword(e.target.value)}
                />
                <Input
                  placeholder="Confirm new password"
                  type="password"
                  pattern="[0-8]*"
                  inputmode="numeric"
                  value={confirmPassword}
                  onChange={(e) => setConfirmPassword(e.target.value)}
                />
                <Button onClick={handlePasswordChange} className="shrink-0">
                  Change Password
                </Button>
              </Flex>
            </Flex>
          </Box>
          <Button colorScheme="red" onClick={handleShutdown}>Shutdown</Button>
        </Flex>

        <Flex flexDirection="column" className="lg:w-1/2 xl:w-2/3" gap={6}>
          <div>
            <Text className="text-3xl">
              <span className="font-bold">System:</span> {systemStatus}
            </Text>
            <Text className="text-3xl">
              <span className="font-bold">Lock:</span> {lockStatus}
            </Text>
            <Text className="text-3xl">
              <span className="font-bold">Current Message:</span> {currentMessage || 'None'}
            </Text>
          </div>
          <div>
            <Text>
              <span className="font-bold">Last updated:</span> {lastUpdated.toLocaleString()}
            </Text>
            <Text>
              <span className="font-bold">Uptime:</span> {!isNaN(uptime) ? showUptime(uptime) : uptime}
            </Text>
            <Flex gap="2" className="items-center">
              <Text className="font-bold">Toggle Lock</Text>
              <Switch size="lg" isChecked={lockStatus === 'locked'} onChange={() => handleLockToggle()} />
            </Flex>
          </div>
          <div className="normal-case">
            <Text className="text-3xl font-bold pb-1">Errors</Text>
            {/* Show last 4 errors */}
            {errors.length
              ? errors.slice(-4).map((error, index) => (
                <Text key={index} className="text-red-600">
                  {error}
                </Text>
              ))
              : 'None! System is running smoothly'}
          </div>
          <div>
            <Text className="text-3xl font-bold pb-2">Recent Events</Text>
            {events.length ? (
              events.slice(0, 10).map((event, index) => (
                <Accordion key={index} allowToggle>
                  <AccordionItem>
                    <h2>
                      <AccordionButton>
                        <Box flex="1" className="flex justify-between max-w-full gap-2">
                          <Text className="font-bold truncate">{event.message}</Text>
                          <Text className="shrink-0">{event.timestamp.toLocaleString()}</Text>
                        </Box>
                        <AccordionIcon />
                      </AccordionButton>
                    </h2>
                    <AccordionPanel pb={4}>
                      <Image src={event.image} alt="Event" boxSize="200px" />
                    </AccordionPanel>
                  </AccordionItem>
                </Accordion>
              ))
            ) : (
              <Text>No recent events</Text>
            )}
          </div>
        </Flex>
      </Flex>
    </Box>
  );
}
