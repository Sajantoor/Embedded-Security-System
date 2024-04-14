#include <iostream>
#include <string>
#include <vector>

#include "common/utils.hpp"
#include "hal/webcam.hpp"
#include "main.hpp"
#include "notifier.hpp"
#include "socket.hpp"

int main(void) {
    Socket socket;
    Webcam webcam(&socket);
    Notifier notifier(&socket);
    
    webcam.startStream();

    for (int i = 0; i < 2; i++){
        notifier.notify(DOOR_CLOSED);
        sleepForMs(1000);
    }

    webcam.stopStream();
    return 0;
}