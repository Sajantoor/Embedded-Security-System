import { useState, useEffect, useRef } from 'react';
import io from 'socket.io-client';
import './index.css';
import {
  Heading,
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

export default function Home() {
  const [cameraStatus, setCameraStatus] = useState('offline');
  const [systemStatus, setSystemStatus] = useState('shutdown');
  const [lockStatus, setLockStatus] = useState('locked');
  const [cameraFeed, setCameraFeed] = useState(null);
  const [lastUpdated, setLastUpdated] = useState(new Date());
  const [displayMessage, setDisplayMessage] = useState('');
  const [currentMessage, setCurrentMessage] = useState('');
  const [password, setPassword] = useState('');
  const [newPassword, setNewPassword] = useState('');
  const [confirmPassword, setConfirmPassword] = useState('');
  const [timeout, setTimeout] = useState(5);
  const [socket, setSocket] = useState(null);
  const [uptime, setUptime] = useState(0);
  const [errors, setErrors] = useState(['System is overheating', 'Camera is offline', 'Lock is jammed']);
  const [events, setEvents] = useState([
    {
      message: 'Multiple Failed Password Attempts',
      timestamp: new Date(),
      image: '/loading.jpg',
    },
    { message: 'System stopped', timestamp: new Date(), image: '/loading.jpg' },
  ]);
  const canvasRef = useRef(null);

  useEffect(() => {
    // Create a socket connection
    const url = 'http://localhost:4000';
    const socket = io(url);

    // setSocket(socket);
     // Listen for connection event
     socket.on("connect", (socket) => {
      console.log("Connected to server");
      setSocket(socket); // Save the socket instance
    });    

    // Listen for incoming messages
    socket.on('message', (message) => {
      handleMessage(message);
    });

    socket.on("canvas", (data) => {
      drawCanvas(data);
    });

    // Clean up the socket connection on unmount
    return () => {
      socket.disconnect();
    };
  }, []);

  function drawCanvas(data) {
    setCameraStatus("online");
    const canvas = canvasRef.current;
    const context = canvas.getContext('2d');
    const image = new window.Image();
    image.src = "data:image/jpeg;base64," + data;
    image.onload = function() {
      context.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas
      context.height = image.height;
      context.width = image.width;
      context.drawImage(image, 0, 0, context.width, context.height);
    };
  }


  function handleMessage(message) {
    console.log('Handling message: ', message);
  }

  function sendMessageToServer(message) {
    if (socket) {
      socket.emit('message', message);
      console.log('Sent message to server: ', message);
    }
  }

  function handleLockToggle() {
    const newLockStatus = lockStatus === 'locked' ? 'unlocked' : 'locked';
    setLockStatus(newLockStatus);

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
    if (timeout) {
      timeoutValue = parseInt(timeout);
    }

    if (timeoutValue > 0) {
      sendMessageToServer(`${DISPLAY_COMMAND} ${displayMessage} ${timeoutValue}`);
    } else {
      sendMessageToServer(`${DISPLAY_COMMAND} ${displayMessage}`);
    }

    setTimeout(5);
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

  return (
    <Box p="10">
      <Flex gap="10" className="flex-col lg:flex-row">
        <Flex flexDirection="column" className="capitalize lg:w-1/2 xl:w-1/3" gap="4">
          {/* <Image src={cameraFeed || '/loading.jpg'} alt="Security Camera" className="" /> */}
          <canvas ref={canvasRef} width="480" height="480"></canvas>
          <Box>
            <Text className="text-3xl font-bold">Show Message</Text>
            <Text className="pb-2 normal-case">Display a message with a timeout. 0 is no timeout.</Text>
            <Flex gap={2}>
              <Input
                placeholder="Enter message"
                value={displayMessage}
                onChange={(e) => setDisplayMessage(e.target.value)}
                maxLength={80}
                className="w-1/2"
              />
              <NumberInput min={0} value={timeout} onChange={setTimeout} className="w-1/2">
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
                type="number"
              />
              <Flex gap={2}>
                <Input
                  placeholder="Enter new password"
                  type="number"
                  value={newPassword}
                  onChange={(e) => setNewPassword(e.target.value)}
                />
                <Input
                  placeholder="Confirm new password"
                  type="number"
                  value={confirmPassword}
                  onChange={(e) => setConfirmPassword(e.target.value)}
                />
                <Button onClick={handlePasswordChange} className="shrink-0">
                  Change Password
                </Button>
              </Flex>
            </Flex>
          </Box>
          <Button colorScheme="red">Shutdown</Button>
        </Flex>

        <Flex flexDirection="column" className="lg:w-1/2 xl:w-2/3" gap={6}>
          <div>
            <Text className="text-3xl">
              <span className="font-bold">System:</span> {systemStatus}
            </Text>
            <Text className="text-3xl">
              <span className="font-bold">Camera:</span> {cameraStatus}
            </Text>
            <Text className="text-3xl">
              <span className="font-bold">Lock:</span> {lockStatus}
            </Text>
            <Text className="text-3xl">
              <span className="font-bold">Message:</span> {currentMessage || 'None'}
            </Text>
          </div>
          <div>
            <Text>
              <span className="font-bold">Last updated:</span> {lastUpdated.toLocaleString()}
            </Text>
            <Text>
              <span className="font-bold">Uptime:</span> {uptime}s
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
              events.slice(-10).map((event, index) => (
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
