#ifndef Maze_h
#define Maze_h

#include <string>

#include "BitVector256.h"
#include "MazeDefinitions.h"
#include "Dir.h"

/**
 * Declaration for a function type that can be used to customize what
 * info is displayed in cells when the maze is drawn.
 * First two arguments are the cell's coordinates.
 * Last argument is the max number of characters that will fit in a cell.
 */
typedef std::string (*cellInfoCallback)(unsigned x, unsigned y, size_t maxInfoLen);

class Maze {
protected:
    BitVector256 wallNS;
    BitVector256 wallEW;
    Dir heading;
    unsigned mouseX = 0;
    unsigned mouseY = 0;

    bool isOpen(unsigned x, unsigned y, Dir d) const;
    void setOpen(unsigned x, unsigned y, Dir d);

public:
    Maze(MazeDefinitions::MazeEncodingName name, unsigned startX = 0, unsigned startY = 0);

    bool wallInFront() const;
    bool wallOnLeft() const;
    bool wallOnRight() const;

    void turnClockwise();
    void turnCounterClockwise();

    // Will throw an exception if the mouse crashes into a wall
    void moveForward();
    void moveBackward();

    std::string draw(const size_t infoLen = 5, cellInfoCallback cb = NULL) const;
};

#endif
