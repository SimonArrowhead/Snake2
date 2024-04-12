This project implements a classic Snake game using NeoPixel LEDs controlled by the Bluepad32 library. The game is played on a grid of NeoPixel LEDs, where the player controls the movement of a snake to eat food and grow longer. The game ends if the snake collides with itself or the boundaries of the grid.

Dependencies:
Bluepad32: A library for interfacing with Bluetooth gamepads on ESP32 platforms.
Adafruit NeoPixel: Library for controlling NeoPixel LEDs.
Adafruit GFX: Core graphics library used by the NeoPixel library.
Adafruit NeoMatrix: Library for controlling NeoMatrix grids of NeoPixels.

Usage:
Connect your NeoPixel LED strip or matrix to the designated pins on your ESP32 board.
Upload the code to your ESP32 board.
Pair your Bluetooth gamepad with the ESP32.
Play the game using the directional buttons on your gamepad to control the snake.

Features:
Dynamic grid rendering on NeoPixel LEDs.
Responsive control using Bluetooth gamepad.
Food generation and snake growth mechanics.
Game over detection for collision with the snake itself or grid boundaries.

Note:
This project uses the Bluepad32 library, which is free to use for open-source projects but requires a commercial license for closed-source projects (more info: https://github.com/ricardoquesada/bluepad32). Make sure to comply with the licensing terms when using this library in your project.
