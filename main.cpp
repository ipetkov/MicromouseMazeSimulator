#include <iostream>
#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"

/**
 * Demo of a PathFinder implementation.
 *
 * Do not use a left/right wall following algorithm, as most
 * Micromouse mazes are designed for such algorithms to fail.
 */
class LeftWallFollower : public PathFinder {
    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward = false;

    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart = false;

    bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

        if(MazeDefinitions::MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return  (x == midpoint     && y == midpoint    ) ||
                (x == midpoint - 1 && y == midpoint    ) ||
                (x == midpoint     && y == midpoint - 1) ||
                (x == midpoint - 1 && y == midpoint - 1);
    }

    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
        const bool frontWall = maze.wallInFront();
        const bool leftWall  = maze.wallOnLeft();

        std::cout << maze.draw() << std::endl << std::endl;

        // If we somehow miraculously hit the center
        // of the maze, just terminate and celebrate!
        if(isAtCenter(x, y)) {
            return Finish;
        }

        // If we hit the start of the maze a second time, then
        // we couldn't find the center and never will...
        if(x == 0 && y == 0) {
            if(visitedStart) {
                return Finish;
            } else {
                visitedStart = true;
            }
        }

        // If we have just turned left, we should take that path!
        if(!frontWall && shouldGoForward) {
            shouldGoForward = false;
            return MoveForward;
        }

        // As long as nothing is in front and we have
        // a wall to our left, keep going forward!
        if(!frontWall && leftWall) {
            shouldGoForward = false;
            return MoveForward;
        }

        // If our forward and left paths are blocked
        // we should try going to the right!
        if(frontWall && leftWall) {
            shouldGoForward = false;
            return TurnClockwise;
        }

        // Lastly, if there is no left wall we should take that path!
        if(!leftWall) {
            shouldGoForward = true;
            return TurnCounterClockwise;
        }

        // If we get stuck somehow, just terminate.
        return Finish;
    }
};

int main(int argc, const char * argv[]) {
    LeftWallFollower leftWallFollower;
    Maze maze(MazeDefinitions::MAZE_CAMM_2012, &leftWallFollower);
    std::cout << maze.draw() << std::endl << std::endl;

    maze.start();
    std::cout << maze.draw() << std::endl << std::endl;
}
