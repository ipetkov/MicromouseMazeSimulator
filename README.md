# A Micromouse Maze Simulator

Useful for testing pathfinding algorithms without having to debug embedded devices.

## Getting started

Create a new class and inherit from the abstract class `PathFinder`. You will need to implement the `MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze)` method so that you can instruct the maze how you want the mouse to navigate.

Then pass an instance of your class to the Maze and call `maze.start()` to start the simulation!

Check out the default `main.cpp` for an example of how to get a simulation running.
