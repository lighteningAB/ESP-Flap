# ESP-Flap Project

## Overview
The ESP-Flap project is designed for the ESP32 microcontroller and includes functionalities for controlling servos, reading acceleration data from an MPU6050 sensor, and interfacing with dual HX711 load cells. 

## Project Structure
```
ESP-Flap
├── main
│   ├── PCA9685_ServoControl.cpp
│   ├── MPU_6050_VelocityPlot.cpp
│   ├── HX711_DualLoadCell.cpp
│   ├── main.c
│   └── CMakeLists.txt
├── CMakeLists.txt
└── README.md
```

## Files Description
- **PCA9685_ServoControl.cpp**: Controls the PCA9685 servo driver, initializes I2C communication, sets PWM values for servo control, and reads user input to adjust servo angles.
- **MPU_6050_VelocityPlot.cpp**: Interfaces with the MPU6050 sensor to read acceleration data, calculates velocity, and prints the results.
- **HX711_DualLoadCell.cpp**: Reads data from two HX711 load cells, initializes GPIO pins for communication, and prints the weight readings.
- **main.c**: The entry point of the application that initializes necessary components and starts the main tasks.
- **CMakeLists.txt**: Used for building the project with CMake, registering source files and include directories.

## Setup Instructions
1. Ensure you have the ESP-IDF environment set up on your machine.
2. Open a terminal and navigate to the project directory.

## Uploading Code to ESP32
1. Run `idf.py build` to compile the project.
2. Connect your ESP32 device to your computer via USB.
3. Run `idf.py -p (PORT) flash` to upload the compiled code to the device, replacing `(PORT)` with the appropriate port (e.g., `/dev/ttyUSB0` on Linux or `COM3` on Windows).
4. After flashing, run `idf.py monitor` to view the output from the ESP32.