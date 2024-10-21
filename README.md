# Details about repository
This repository contains a project for a hardware-based Rock Paper Scissors game using Arduino Uno and C++. The game logic and decision-making will run primarily on the Arduino Uno, while the C++ QT application will serve as a graphical interface for users to interact with the game.
 

# Task details
## Task 1
Main goals to this task is to create repository with main branch **develop**. Then create branch with name **feature/develop/task1** and create `README.md` file with all descriptions. Also create **TAG** and make pull request.

## Student details
| Student number | Game | config format |
| :-----------: | :-------------: | :-----------: |
| 16 | rock paper scissors | XML |

## Technology Stack and Hardware Used

### Hardware
- **Arduino Uno**: The Arduino will handle most of the game logic. For example, to determine the winner, save the current game status and more. The graphical will be developed using Qt.

### Software
- **QT Creator**: Used for creating the graphical user interface (GUI) that displays the game board and allows users to view the game progress in real time.
- **Platformio**: To write and upload the logic code to the Arduino Uno, primarily using C/C++ for low-level control.

### Programming Languages
- **C/C++**: Used in the Arduino environment to develop the Rock Paper Scissors game logic and in graphical interface.
### Communication
- **Serial Communication**: The Arduino will communicate with the app through a UART serial port to send and receive game status and input data.

## Task2
Main goal: Create a simple communication schema SW(client) <-> UART <-> HW(server). The client should send a message to the server. The server should modify the message and send it back to the client.

## Build client
You need to install Qt Creator and CMake. Then you need to run next commands in cmd for example:
```
mkdir build
cd build
cmake ..
cmake --build .
```
## Build server
You need to install PlatformIO and use its GUI for build and run project, or run next command:
```
pio run
```