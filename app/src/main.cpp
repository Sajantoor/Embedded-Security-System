
#include "hal/webcam.hpp"
#include "socket.hpp"

int main(void) {
    Socket socket;
    Webcam webcam(&socket);

    webcam.startStream();

    socket.sendToWebServer("Hello from the BeagleBone!");

    socket.closeSocket();
    webcam.stopStream();
    return 0;
}