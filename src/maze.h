/* Your code here! */
#pragma once

// MOST RECENT VERSION

#include <vector>
#include <stdlib.h>
/*#include <time.h>
#include <ctime>*/
#include <queue>
#include "dsets.h"
#include "cs225/PNG.h"
using namespace std;
using namespace cs225;

class SquareMaze{
    public:
    SquareMaze();
    bool canTravel(int x, int y, int dir) const;
    PNG* drawMaze() const;
    PNG* drawMazeWithSolution();
    void makeMaze(int width, int height);
    void setWall(int x, int y, int dir, bool exists);
    vector<int> solveMaze();
    //~SquareMaze();

    private:
    int width_;
    int height_;
    //PNG* maze;
    vector<int> solution;
    vector<vector<pair<bool, bool>>> maze_encoding;
    DisjointSets connections;
    int maxrow;

};