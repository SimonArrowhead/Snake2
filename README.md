**This project implements a classic Snake game using NeoPixel LEDs controlled by the Bluepad32 library. The game is played on a grid of NeoPixel LEDs, where the player controls the movement of a snake to eat food and grow longer. The game ends if the snake collides with itself or the boundaries of the grid.**

## Dependencies:
- Bluepad32: A library for interfacing with Bluetooth gamepads on ESP32 platforms.
- Adafruit NeoPixel: Library for controlling NeoPixel LEDs.
- Adafruit GFX: Core graphics library used by the NeoPixel library.
- Adafruit NeoMatrix: Library for controlling NeoMatrix grids of NeoPixels.

## Usage:
Connect your NeoPixel LED strip or matrix to the designated pins on your ESP32 board.
Upload the code to your ESP32 board.
Pair your Bluetooth gamepad with the ESP32.
Play the game using the directional buttons on your gamepad to control the snake.

## Features:
Dynamic grid rendering on NeoPixel LEDs.
Responsive control using Bluetooth gamepad.
Food generation and snake growth mechanics.
Game over detection for collision with the snake itself or grid boundaries.

## Instructions
1. Install the Bluepad library by following the instructions in the repository.
2. Make sure to include the necessary libraries (`Adafruit_NeoPixel`, `Adafruit_GFX`, `Adafruit_NeoMatrix`) in your Arduino IDE.
3. Set up your LED matrix display according to your hardware configuration. Modify the `twoDArray` variable to match the layout of your LED matrix. Each element in the array represents a pixel on the display, with `-1` indicating empty space.
4. Adjust the size and layout of the LED matrix in the `rows` and `cols` variables.
5. Customize the snake's starting length and initial position by modifying the `sLengthControl` vector. Ensure that the length matches the starting position of the snake on the LED matrix.
6. Compile and upload the code to your ESP32 board.
7. Connect your Bluetooth gamepad to the ESP32 board using the Bluepad library.
8. Enjoy playing the Snake game using the gamepad controls!

## Compatibility and Adjustments for Arduino:
While this project is primarily designed for ESP32 boards due to their dual-core capabilities, it can also be adapted to work with Arduino boards. However, some adjustments are necessary, especially if using a smaller LED matrix.

If using an Arduino board:
- Note that the matrix size may need to be significantly smaller due to memory and processing constraints.
- Additionally, the border refreshing mechanism may need to be modified to ensure smooth operation on single-core Arduino boards.
Ensure to adjust the code accordingly and test thoroughly to achieve optimal performance on your Arduino board.

## License
This program is open source and licensed under the Apache License 2.0. You are free to modify and distribute the code under the terms of this license. However, please note that this project utilizes the Bluepad32 library, which in turn relies on the BTstack library. While BTstack is freely available for open-source projects, a commercial license is required for closed-source projects (more information: https://github.com/ricardoquesada/bluepad32). Ensure compliance with the licensing terms when incorporating this library into your project.


