# RaylibPongGame

## A simple Pong Game made with C++ and Raylib

## About Project
This was a game that I made to practice raylib with c++. It is similar to a classic pong game with a few changes.
* A ball that bounces off window edges and the paddle objects, that speeds up with alternate bounces, using vector reflection for direction
* 2 paddles that move up down and uses Continuos Collision detection, which checks if the line segment that follows the path of the ball intersects with the paddle, instead of checking the overlapping of the ball itself
* Header file based organization
* The game is 2 player. So one player uses W and S while the other uses up and down arrow keys

![image](https://github.com/user-attachments/assets/7be0faf6-7056-4cc5-86c0-f19992ffc03d)

## Installation Instructions
This build is exclusive to the linux environment. For other OSes, ensure the make file is updated or run the required command
* Download the repository
*  Run the make file
*  Run the generated executable `main`

## Known Issues
* File organization and coding practices are not the best
* While most collision bugs have been fixed, some can still occur
* There is no gameplay design or main menu
