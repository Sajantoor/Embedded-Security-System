# Embedded Security System

This project is a security system which can be installed in an existing door.
The system features hardware components like a motorised lock, keypad, LCD display,
motion sensor, camera, joystick and buzzer. The system includes 2 major components,
the BeagleBone Green software (in the root directory) and the web-app (in web-app).

![View of our hardware](https://github.com/Sajantoor/CMPT433-Project/assets/25991050/5345f525-55eb-4739-8802-8f293897ffd6)

This is a project for CMPT 433: Embedded Systems at Simon Fraser University.

## System Overview

### Hardware and Software Libraries Used:

**Software**:

-   React.js, Tailwind: For web app front end
-   Node.js: For web app backend
-   Socket IO
-   FFmpeg: For webcam streaming

**Hardware**:

-   Solenoid lock
-   Relay
-   Motion Sensor
-   USB Logitech C270 webcam
-   12 Key keypad
-   LCD screen

The joystick is used as a toggle to unlock and lock the door. To unlock, the user
must click the joystick and enter the password on the keypad. The password being
entered by the user on the keypad will display on the LCD. if they get the
correct password the system will unlock, otherwise the system will notify the
user with the LCD and buzzer.

![LCD](https://github.com/Sajantoor/CMPT433-Project/assets/25991050/6c12bd5e-9938-4166-bc30-068ae314fd5f)

The LCD does more than just display the password being entered by the user. By
default, it shows the status of the lock as well. We can also display any message
we want using the web app interface, this is particularly useful for cases where
someone is at the door and you want to communicate with them or you’re expecting
a package and want to inform the deliverer of any special instructions. The display
message can also be timed out after a certain amount of time. For longer messages,
the LCD scrolls. We tested with O’Canada and the Bee Movie Script and were able to
display both without any issues. An example of scrolling text can be seen
[![in this video](https://img.youtube.com/vi/LvgnJA_K-YI/maxresdefault.jpg)](https://youtu.be/LvgnJA_K-YI).



The system also features many surveillance features. With the webcam, we are able
to get a live video feed of what’s happening at the door within the web app.
The motion sensor is used to report any significant motion happening at the door.
Significant motion is determined by a threshold overtime (see code for more details).
We also have events, such a motion detected, door opened, door closed and multiple
failed password attempts. These notify the user of the current state of the door or
what’s happening at the door. They also include a picture to show what’s happening.

Security is very important for a security system (obviously!), so we have 2 important
security features. When passwords are created for the system, they are done locally on
the BBG. A password is created when the security system software is first launched and
the user is prompted (on the LCD) to create a password using the keypad. These passwords
are hashed and stored on a file on the BBG. All comparisons are made against the hashed
password. When a password is changed, the password is also encrypted. Password changes
are possible by inputting up on the joystick or through the web app, and both require
the previous password. The next security feature we have is to prevent brute force attempts
on the lock. If 3 failed password attempts are made in a row, we notify the user. The user
is also notified for subsequent failed attempts. At 5 failed password attempts and above,
the system “locks up” and enters a disabled state for 2 minutes. No input from either
the web server or the device itself is accepted during this disabled state. The LCD shows
the device is disabled and counts down the 2 minute timer. Webcam footage and all other
events still go to the web-app.

Another feature we have is Discord notifications, which are another way to see events.

![Web App](https://github.com/Sajantoor/CMPT433-Project/assets/25991050/5666e04d-5ef9-47b9-9d8e-b2ffc3ae9932)

The web-app shows the live camera feed, status of the BBG (whether it's online or not), current message, lock status, events, up time and last updated.

## How It Works

![Project Diagram](https://github.com/Sajantoor/CMPT433-Project/assets/25991050/3a24fa14-1547-45e9-867b-9a8c82366495)

The motion sensor works by using A2D, its value is checked on another thread. If the motion sensor value is above the threshold, then it increments motion count. If the motion count is greater than the threshold, then it sends a notification. Subsequent notifications are sent out after 2 minutes. This is how we detect significant motion, ie, we want to detect people, not leaves blowing in the wind.

The keypad module runs on a separate thread and continuously reads through all keys using GPIO, to determine whether any keys have been pressed. There is a debounce to ensure that each key pressed is only recorded once. The keypad module is utilised in the display manager, which displays the keypad inputs to the LCD display. The implementation is somewhat similar to the Joystick module, which also employs reading GPIO values to determine whether the joystick has been pressed or shifted.

The LCD module was used to display messages to the user as they use the lock. It uses GPIO pins and the BeagleBone communicates with the LCD through bit-banging. We decided to use the LCD in 4-bit mode, so data is sent only through pins DB4-DB7, instead of from DB0-DB7. First, the RS pin is set to 1. This lets the LCD know that data is to be written. Then the upper four bits of a byte are sent, followed by pulsing the E pin to let the LCD know to store said pins, before rewriting DB4-DB7 with the lower four bits. Again, the E pin is pulsed to store the four bits. Finally, the RS pin is set back to 0. The message can be displayed on the LCD in two ways. If the message is less than or equal to 32 characters, it’s displayed on the LCD such that 16 characters show up on each line. For longer messages, we use only one line, but set a thread to keep scrolling through the message until it is overwritten by a new message. Like the keypad, the LCD module is also utilised by the display manager.

The relay module handles the opening and closing of the lock. Initially the program will close the lock. In addition to having the lock open or close we have a function to check the status of the lock. The relays are controlled using the grove I2C connector. The status of the lock is updated on the LCD, as well as the web client as it is changed.

The webcam module handles getting data from the webcam and sending it in packets to the server. This module runs in a thread on its own, and continuously sends packets to the server as it reads them from the webcam. The resolution being retrieved from the webcam and sent to the server are set here. The socket used to send data to the server is the same one as used in other parts of the program. We pass it in by reference to make it easier and safer to manage in the different parts of the program. As the BBG gets data from the webcam it will update and send it to the server.

The heartbeat module ensures that the system is alive by sending UDP messages, including the currently displayed message and up time every second on a separate thread.

The password module creates, stores, hashes, and compares passwords. Passwords are stored on a file on the BBG and are hashed before being stored.

We use a Node.js server for communication with the BBG over UDP. It acts as a UDP / WebSocket relay to communicate with the web client. It also sends messages to Discord and processes the incoming webcam stream using FFmpeg.

We use a React.js front end with Tailwind and Chakra UI to display a client page to the user. This allows the user to see the updated status of the system. As the server gets UDP messages, it sends data to the client via WebSockets. This includes webcam stream, lock status, as well as recent events. In addition to seeing the status, the user can enter a message here to display on the LCD as well as change their password. The client page is the only place where the user is able to shutdown the program. The recent events are in list form, but can be selected to show a drop down with an image which was captured when the event took place. The lock can be toggled here as well to allow the user to open the lock in case they want to let someone in.

## Code Overview and Structure

### Compiler

This assignment uses C++ in lieu of C.
Run `sudo apt-get install g++-arm-linux-gnueabihf` and set the CMake compiler to `/usr/bin/arm-linux-gnueabihf-g++`

### Formatting

The project contains a `.clang-format` file. Run `sudo apt install clang-format`.
Ensure you download the `Clang-Format` vscode extension, set it as the default formatter, and set it to automatically format on save.

### Structure

-   `hal/`: Contains all low-level hardware abstraction layer (HAL) modules
-   `app/`: Contains all application-specific code. Broken into modules and a main file
-   `build/`: Generated by CMake; stores all temporary build files (may be deleted to clean)
-   'web-app/': Contains the web-app client and server for this project

```
  .
  ├── app
  │   ├── include
  │   │   └── badmath.h
  │   ├── src
  │   │   ├── badmath.c
  │   │   └── main.c
  │   └── CMakeLists.txt           # Sub CMake file, just for app/
  ├── hal
  │   ├── include
  │   │   └── hal
  │   │       └── button.h
  │   ├── src
  │   │   └── button.c
  │   └── CMakeLists.txt           # Sub CMake file, just for hal/
  ├── CMakeLists.txt               # Main CMake file for the project
  └── README.md
```

### Usage

-   Install CMake: `sudo apt update` and `sudo apt install cmake`
-   When you first open the project, click the "Build" button in the status bar for CMake to generate the `build\` folder and recreate the makefiles.
    -   When you edit and save a CMakeLists.txt file, VS Code will automatically update this folder.
-   When you add a new file (.h or .c) to the project, you'll need to rerun CMake's build
    (Either click "Build" or resave `/CMakeLists.txt` to trigger VS Code re-running CMake)
-   Cross-compile using VS Code's CMake addon:
    -   The "kit" defines which compilers and tools will be run.
    -   Change the kit via the menu: Help > Show All Commands, type "CMake: Select a kit".
        -   Kit "GCC 10.2.1 arm-linux-gnueabi" builds for target.
        -   Kit "Unspecified" builds for host (using default `gcc`).
    -   Most CMake options for the project can be found in VS Code's CMake view (very left-hand side).
-   Build the project using Ctrl+Shift+B, or by the menu: Terminal > Run Build Task...
    -   If you try to build but get an error about "build is not a directory", the re-run CMake's build as mentioned above.

## Team Members:

-   [Sajan Toor](https://github.com/sajantoor)
-   [Garry Chahal](https://github.com/garrysc)
-   [Jimmy Nimavat](https://github.com/adpops)
-   [Sukhpal Gosal](https://github.com/sukhpalgosal)
