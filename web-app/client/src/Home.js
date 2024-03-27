
import { useState, useEffect } from "react";
import io from "socket.io-client";
import './App.css';
import { Heading, Center, Text, Image, Box, Switch } from "@chakra-ui/react";

export default function Home() {
  const [cameraStatus, setCameraStatus] = useState("offline");
  const [lockStatus, setLockStatus] = useState("locked");
  const [cameraFeed, setCameraFeed] = useState(null);
  const [lastUpdated, setLastUpdated] = useState(null);
  const [currentMessage, setCurrentMessage] = useState("");
  const [socket, setSocket] = useState(null);
  const [uptime, setUptime] = useState(null);

  useEffect(() => {
    // Create a socket connection
    const url = "http://localhost:4000";
    const socket = io(url);


    setSocket(socket);

    // Clean up the socket connection on unmount
    return () => {
      socket.disconnect();
    };
  }, []);

  useEffect(() => {
    if (!socket) {
      return;
    }

    // Listen for incoming messages
    socket.on("message", (message) => {
      handleMessage(message);
    });
  });

  function handleMessage(message) {
    console.log(message);
  }

  return (
    <Box p="20">
      <Center>
        <Heading> Security Camera Status: {cameraStatus} </Heading>
      </Center>

      <Center>
        <Heading> Door Lock Status: {lockStatus} </Heading>
      </Center>

      <Image src={cameraFeed || "/camera.webp"} alt="Security Camera" />
      <Text> Last updated: {lastUpdated} </Text>
      <Text> Uptime: {uptime} </Text>

      <Box>
        <Text> Toggle Lock </Text>
        <Switch size="lg"
          isChecked={lockStatus === "locked"}
          onChange={() => setLockStatus(lockStatus === "locked" ? "unlocked" : "locked")} />
      </Box>
    </Box>
  );
}


