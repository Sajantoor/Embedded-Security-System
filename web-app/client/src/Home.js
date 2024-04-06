
import { useState, useEffect, useRef } from "react";
import io from "socket.io-client";
import './App.css';
import { Heading, Center, Text, Image, Box, Switch, Input, Button } from "@chakra-ui/react";

const DISPLAY_COMMAND = "setDisplayMessage";
const LOCK_COMMAND = "lock";
const UNLOCK_COMMAND = "unlock";
const CHANGE_PASSWORD_COMMAND = "changePassword";

export default function Home() {
  const [cameraStatus, setCameraStatus] = useState("offline");
  const [lockStatus, setLockStatus] = useState("locked");
  const [cameraFeed, setCameraFeed] = useState(null);
  const [lastUpdated, setLastUpdated] = useState(null);
  const [currentMessage, setCurrentMessage] = useState("");
  const [password, setPassword] = useState("");
  const [newPassword, setNewPassword] = useState("");
  const [confirmPassword, setConfirmPassword] = useState("");
  const [timeout, setTimeout] = useState("");
  const [socket, setSocket] = useState(null);
  const [uptime, setUptime] = useState(null);
  const canvasRef = useRef(null);

  useEffect(() => {
    // Create a socket connection
    const url = "http://localhost:4000";
    const socket = io(url);

    // setSocket(socket);
     // Listen for connection event
     socket.on("connect", (socket) => {
      console.log("Connected to server");
      setSocket(socket); // Save the socket instance
    });    

    // Listen for incoming messages
    socket.on("message", (message) => {
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
    console.log("Handling message: ", message);
  }

  function sendMessageToServer(message) {
    if (socket) {
      socket.emit("message", message);
      console.log("Sent message to server: ", message);
    }
  }

  function handleLockToggle() {
    const newLockStatus = lockStatus === "locked" ? "unlocked" : "locked";
    setLockStatus(newLockStatus);

    if (newLockStatus === "locked") {
      sendMessageToServer(LOCK_COMMAND);
    } else {
      sendMessageToServer(UNLOCK_COMMAND);
    }
  }

  function handleChangeMessage() {
    if (!currentMessage) {
      return;
    }

    let timeoutValue = 0;
    if (timeout) {
      timeoutValue = parseInt(timeout);
    }

    if (timeoutValue > 0) {
      sendMessageToServer(`${DISPLAY_COMMAND} ${currentMessage} ${timeoutValue}`);
    } else {
      sendMessageToServer(`${DISPLAY_COMMAND} ${currentMessage}`);
    }
  }

  function handlePasswordChange() {
    if (!password || !newPassword || !confirmPassword) {
      return;
    }

    if (newPassword !== confirmPassword) {
      alert("Passwords do not match");
      return;
    }

    sendMessageToServer(`${CHANGE_PASSWORD_COMMAND} ${password} ${newPassword}`);
  }

  return (
    <Box p="20">
      <Center>
        <Heading> Security Camera Status: {cameraStatus} </Heading>
      </Center>

      <Center>
        <Heading> Door Lock Status: {lockStatus} </Heading>
      </Center>

      <canvas ref={canvasRef} width="480" height="480"></canvas>

      <Text> Last updated: {lastUpdated} </Text>
      <Text> Uptime: {uptime} </Text>

      <Box>
        <Text> Toggle Lock </Text>
        <Switch size="lg"
          isChecked={lockStatus === "locked"}
          onChange={() => handleLockToggle()} />
      </Box>

      <Box>
        <Input plassholder="Enter message" value={currentMessage} onChange={(e) => setCurrentMessage(e.target.value)} />
        <Input type="number" placeholder="Enter timeout" onChange={(e) => setTimeout(e.target.value)} />
        <Button onClick={() => handleChangeMessage(currentMessage)}> Send Message </Button>
      </Box>

      <Box>
        <Heading> Change Password </Heading>
        <Input placeholder="Enter current password" type="password" onChange={(e) => setPassword(e.target.value)} />
        <Input placeholder="Enter new password" type="password" onChange={(e) => setNewPassword(e.target.value)} />
        <Input placeholder="Confirm new password" type="password" onChange={(e) => setConfirmPassword(e.target.value)} />
        <Button onChange={() => handlePasswordChange()}> Change Password </Button>
      </Box>
    </Box>
  );
}


