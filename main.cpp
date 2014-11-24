#include <iostream>
#include <unistd.h> // getopt

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
public:
    LeftWallFollower(bool shouldPause = false) : pause(shouldPause) {}

    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
        const bool frontWall = maze.wallInFront();
        const bool leftWall  = maze.wallOnLeft();

        // Pause at each cell if the user requests it.
        // It allows for better viewing on command line.
        if(pause) {
            std::cout << "Hit enter to continue..." << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.clear();
        }

        std::cout << maze.draw() << std::endl << std::endl;

        // If we somehow miraculously hit the center
        // of the maze, just terminate and celebrate!
        if(isAtCenter(x, y)) {
            std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
            return Finish;
        }

        // If we hit the start of the maze a second time, then
        // we couldn't find the center and never will...
        if(x == 0 && y == 0) {
            if(visitedStart) {
                std::cout << "Unable to find center, giving up." << std::endl;
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
        std::cout << "Got stuck..." << std::endl;
        return Finish;
    }

protected:
    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward = false;

    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart = false;

    // Indicates we should pause before moving to next cell.
    // Useful for command line usage.
    bool pause = false;


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
};

int main(int argc, char * argv[]) {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = false;

    int opt;
    opterr = 0;

    while((opt = getopt(argc, argv, "m:ph")) != -1) {
        switch(opt) {
            case 'm': {
                int mazeOption = atoi(optarg);
                if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
                    mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
                }
                break;
            }

            case 'p':
                pause = true;
                break;

            case 'h':
            default:
                std::cout << "Usage: " << argv[0] << " [-m N] [-p]" << std::endl;
                std::cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << std::endl;
                std::cout << "\t-p will wait for a newline in between cell traversals" << std::endl;
                return -1;
        }
    }

    LeftWallFollower leftWallFollower(pause);
    Maze maze(mazeName, &leftWallFollower);
    std::cout << maze.draw() << std::endl << std::endl;

    maze.start();
}
