import { useState, useEffect } from 'react';
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
const SHUTDOWN = 'shutdown';

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
  const [events, setEvents] = useState([]);
  // most likely not going to use this
  const [errors, setErrors] = useState(['System is overheating', 'Camera is offline', 'Lock is jammed']);

  useEffect(() => {
    const url = 'http://localhost:4000';
    const socket = io(url);

    setSocket(socket);

    socket.on('connect', () => {
      console.log('Connected to server');
    });

    socket.on('event', (event) => {
      const date = new Date();
      date.setUTCSeconds(event.epochTime);
      event.timestamp = date;
      event.image = '/loading.jpg';

      // if the event message contains 'door open' or 'door close', setDoorStatus
      if (event.message.includes('Door Opened')) {
        setLockStatus('unlocked');
      } else if (event.message.includes('Door Closed')) {
        setLockStatus('locked');
      } else if (event.message.includes('Display Message Set')) {
        setCurrentMessage(displayMessage);
      }

      setEvents((prevEvents) => [...prevEvents, event]);
    });

    socket.on('uptime', (uptime) => {
      setUptime(uptime);
      setSystemStatus('online');
    });

    // Clean up the socket connection on unmount
    return () => {
      socket.disconnect();
    };
  }, []);

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
      sendMessageToServer(`${DISPLAY_COMMAND} ${timeoutValue} ${displayMessage}`);
    } else {
      sendMessageToServer(`${DISPLAY_COMMAND} 0 ${displayMessage}`);
    }

    setTimeout(0);
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
    setSystemStatus('shutdown');
  }

  return (
    <Box p="10">
      <Flex gap="10" className="flex-col lg:flex-row">
        <Flex flexDirection="column" className="capitalize lg:w-1/2 xl:w-1/3" gap="4">
          <Image src={cameraFeed || '/loading.jpg'} alt="Security Camera" className="" />
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
          <Button colorScheme="red" onClick={handleShutdown}>Shutdown</Button>
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
