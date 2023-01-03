/* Your code here! */
#include "maze.h"
#include "iostream"

using namespace std;
using namespace cs225;

// MOST RECENT VERSION

SquareMaze::SquareMaze() {
    // creates an empty maze
    connections = DisjointSets();
    width_ = 0;
    height_ = 0;
    maxrow = 0;
    //srand(time(NULL));
}

/*SquareMaze::~SquareMaze() {
	delete maze;
    width_ = 0;
    height_ = 0;
    maxrow = 0;
	//maze = NULL;
}*/

bool SquareMaze::canTravel(int x, int y, int dir) const {
    // determines whether its possible to travel in given direction from (x, y)
    // dir = 0: right (x += 1)
    // dir = 1: down (y += 1)
    // dir = 2: left (x -= 1)
    // dir = 3: up (y -= 1)
    // cannot step off maze or through wall; returns false if impossible
    if(x >= width_ || y >= height_ || x < 0 || y < 0 || dir < 0 || dir > 3){
        return false;
    }
    if(dir == 0){
        return (maze_encoding[y][x].first == false);
    } else if(dir == 1){
        return (maze_encoding[y][x].second == false);
    } else if(dir == 2){
        return (x != 0 && (maze_encoding[y][x-1].first == false));
    } else if(dir == 3){
        return (y != 0 && (maze_encoding[y-1][x].second == false));
    }
    return false;
}

PNG* SquareMaze::drawMaze() const {
    // draws maze without solution:
    // create new PNG with dimensions width*10+1; height*10+1 (height + width = args to makemaze)
    // blacken top left + leftmost column except entrance (1, 0) - (9, 0)
    // for each square (x, y) in maze:
    // if right wall exists, blacken pixels ((x+1)*10, y*10+k) for k: 0 - 10.
    // if bottom wall exists, blacken pixels (x*10+k, (y+1)*10) for k: 0 - 10
    PNG* image = new PNG(width_*10+1, height_*10+1);
    for(int x = 0; x < width_*10; x++){
        for(int y = 0; y < height_*10; y++){
            if(x % 10 == 0){
                if(y % 10 == 0){
                    image->getPixel(x, y) = HSLAPixel(0,0,0);
                } else if (maze_encoding[(int)(y/10)][(int)(x/10)].first){
                    image->getPixel(x+10, y) = HSLAPixel(0,0,0);
                }
            } else if(y%10==0){
                if(maze_encoding[(int)(y/10)][(int)(x/10)].second){
                    image->getPixel(x, y+10) = HSLAPixel(0,0,0);
                }
            }
        }
    }
    for(int x = 0; x < height_*10+1; x++){
        image->getPixel(x, width_*10) = HSLAPixel(0,0,0);
    }
    for(int y = 0; y < (width_-1)*10+1; y++){
        image->getPixel(height_*10, y) = HSLAPixel(0,0,0);
    }
    for(int i = 10; i < width_*10+1; i++){
        image->getPixel(i, 0) = HSLAPixel(0,0,0);
    }
    for(int i = 0; i < height_*10+1; i++){
        image->getPixel(0, i) = HSLAPixel(0,0,0);
    }
    return image;
}

PNG* SquareMaze::drawMazeWithSolution() {
    // calls drawmaze then solvemaze
    // modifies PNG to show solution vector and exit
    // each direction in solution vector corresponds to 11 red pixels in given direction
    // start at (5, 5); make exit by undoing bottom wall of dest square: whiten (x*10+k, (y+1)*k) k: 1 - 9
    //cout<<"i am being called"<<endl;
    PNG* maze = drawMaze();
    solution = solveMaze();
    int curx = 5;
    int cury = 5;
    for(unsigned i = 0; i < solution.size(); i++){
        if(solution[i] == 0 && curx<=10*(width_-1)){
            for(int k = 0; k < 11; k++){
                maze->getPixel(curx+k, cury) = HSLAPixel(0,1,0.5,1);
            }
            if(curx+10 < (int)width_*10 + 1){
                curx = curx+10;
            }
        } else if (solution[i] == 1 && cury <= 10*(height_-1)){
            for(int k = 0; k < 11; k++){
                maze->getPixel(curx, cury+k) = HSLAPixel(0,1,0.5,1);
            }
            if((cury+10) < (int)height_*10 + 1){
                cury = cury+10;
            }
        } else if (solution[i] == 2 && curx >= 10){
            for(int k = 0; k < 11; k++){
                maze->getPixel(curx-k, cury) = HSLAPixel(0,1,0.5,1);
            }
            if((curx - 10) >= 0){
                curx = curx-10;
            }
        } else if (solution[i] == 3 && cury >= 10){
            for(int k = 0; k < 11; k++){
                maze->getPixel(curx, cury-k) = HSLAPixel(0,1,0.5,1);
            }
            if((cury - 10) >= 0){
                cury = cury-10;
            }
        } 
    }
    //cout<<"NSF"<<endl;
    for(int i = 1; i < 10; i++){
        maze->getPixel(10*maxrow+i, height_*10) = HSLAPixel(0,0,1,1);
    }
    //cout<<"NSF2"<<endl;
    
    return maze;

}

void SquareMaze::makeMaze(int width, int height) {
    // makes new square maze of given width and height;
    // start with square grid; select random walls to delete without creating a cycle (do not delete perimeter walls)
    // 2 bits per square : down, right
    /*if(maze != NULL){
        delete maze;
        maze = NULL;
    }*/
    //maze = new PNG();
    width_ = width;
    height_ = height;
    //maze = new PNG(width_*10 + 1, height_*10  + 1);
    //connections = DisjointSets();
    connections.addelements(width_ * height_);

    pair<bool, bool> defpair = pair<bool, bool>(true, true);
    vector<pair<bool, bool>> defvec = vector<pair<bool, bool>>(width_, defpair);
    maze_encoding = vector<vector<pair<bool, bool>>>(height_, defvec);

    //cout<< width_<< " " << height_ << " " << (width_*height_) << endl;
    //int urmom = 0;
    while(connections.largest_tree < (int)width_*height_){
        //int dir = 0;
        /*int r = rand() % (width_*height_*2);
        if(r >= width_*height_){
            r = r % (width_*height_);
            dir = 1;
        }
        cout<<r<<endl;

        setWall(r - ((int)(r/width_))*width_, (int)(r/width_), dir, false);*/
        int x = rand() % (width_);
        int y = rand() % (height_);
        unsigned dir = rand() % 2;
        //cout << rand_x << " " << rand_y << " " << dir << endl;
        //int x = urmom%width_;
        //int y = (int)(urmom/width_);
        if(dir == 0 && x != width_ - 1 && (connections.find(width_*y + x) != connections.find((width_*y) + x + 1))){
            connections.setunion((width_*y) + x, (width_*y)+x+1);
            setWall(x, y, dir, false);
        }
        if(dir == 1 && y != height_ - 1 && (connections.find((width_*(y+1)) + x) != connections.find((width_*y) + x))){
            connections.setunion((width_*y)+x, ((width_)*(y+1))+x);
            setWall(x, y, dir, false);
        }

        //urmom = urmom+1;
        //urmom = urmom % (width_*height_);
    }
    //connections.printset();
    //maze = drawMaze();

}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    // sets a wall to be present or not present (should be O(n) !!)
    // dir = 0: right; dir = 1: down
    // exists: true if setting wall to exist; false if else
    if(dir == 0){
        if(x == (width_ - 1)){
            return;
        }
        if(!exists){
            maze_encoding[y][x].first = false;
        } else if(exists){
            maze_encoding[y][x].first = true;
        }
        return;
    }
    if(dir == 1){
        if(y == (height_ - 1)){
            return;
        }
        if(!exists){
            maze_encoding[y][x].second = false;
        } else if(exists){
            maze_encoding[y][x].second = true;
        }
        return;
    }
}

vector<int> SquareMaze::solveMaze() {
    // each square in bottom row has distance to origin == length of only path through maze from origin to that square
    // square with longest path = destination
    // return winning path as vector of integers, representing directions from origin to winning square
    // in case of tie: use smallest x val (leftmost coord)
    
    //current node = 0,0 (pair of ints)
    //if cantravel in any direction: push next node to queue
    

    /*vector<vector<int>> solutions;

    queue<vector<int>> bfs;
    vector<int> start = vector<int>(2, 0);
    bfs.push(start);
    while(!bfs.empty() && ((int)solutions.size() != width_)){
        vector<int> cur = bfs.front();
        bfs.pop();
        if(cur[0] == width_ - 1 && cur[1] == height_ - 1){
            break;
        }
        
        if(canTravel(cur[0], cur[1], 0)){
            vector<int> to_push = cur;
            to_push[0] = cur[0]+1;
            to_push.push_back(0);
            bfs.push(to_push);
        }
        if(canTravel(cur[0], cur[1], 1)){
            vector<int> to_push = cur;
            to_push[1] = cur[1]+1;
            to_push.push_back(1);
            bfs.push(to_push);
        }
        if(canTravel(cur[0], cur[1], 2)){
            vector<int> to_push = cur;
            to_push[0] = cur[0]-1;
            to_push.push_back(2);
            bfs.push(to_push);
        }
        if(canTravel(cur[0], cur[1], 3)){
            vector<int> to_push = cur;
            to_push[1] = cur[1]-1;
            to_push.push_back(3);
            bfs.push(to_push);
        }
        if(cur[1] == height_-1){
            solutions.push_back(cur);
        }
    }
    solution = solutions[0];
    for(unsigned i = 0; i < solutions.size(); i++){
        if(solutions.size() < solutions[i].size()){
            solution = solutions[i];
        }
    }

    vector<int> newsol;
    for(unsigned i = 2; i < solution.size(); i++){
        newsol.push_back(solution[i]);
        cout << solution[i] << " ";
    }
    cout << endl;
    solution = newsol;
    return solution;*/

    vector<int> defaultrow = vector<int>(width_, -1);

    vector<vector<int>> grid = vector<vector<int>>(height_, defaultrow);

    queue<pair<int, int>> bfs;
    grid[0][0] = 0;
    pair<int, int> start;
    start.first = 0;
    start.second = 0;
    bfs.push(start);
    while(!bfs.empty()){
        pair<int, int> cur = bfs.front();
        int x = cur.first;
        int y = cur.second;
        bfs.pop();
        if(canTravel(x, y, 0) && grid[y][x+1] == -1){
            pair<int, int> to_push;
            to_push.first = x + 1;
            to_push.second = y;
            bfs.push(to_push);
            grid[y][x+1] = grid[y][x] + 1;
        }
        if(canTravel(x, y, 1) && grid[y+1][x] == -1){
            pair<int, int> to_push;
            to_push.first = x;
            to_push.second = y+1;
            bfs.push(to_push);
            grid[y+1][x] = grid[y][x] + 1;
        }
        if(canTravel(x, y, 2) && grid[y][x-1] == -1){
            pair<int, int> to_push;
            to_push.first = x - 1;
            to_push.second = y;
            bfs.push(to_push);
            grid[y][x-1] = grid[y][x] + 1;
        }
        if(canTravel(x, y, 3) && grid[y-1][x] == -1){
            pair<int, int> to_push;
            to_push.first = x;
            to_push.second = y-1;
            bfs.push(to_push);
            grid[y-1][x] = grid[y][x] + 1;
        }
    }

    //print grid [for testing]
    /*for(int x = 0; x < width_; x++){
        for(int y = 0; y < height_; y++){
            cout << grid[y][x] << " ";
        }
        cout<<endl;
    }*/

    int masize = 0;
    int maxidx = 0;
    for(int i = 0; i < width_; i++){
        if(grid[height_-1][i] > masize){
            maxidx = i;
            masize = grid[height_-1][i];
        }
    }

    int curx = maxidx;
    int cury = height_-1;
    maxrow = maxidx;
    //cout<<masize<<endl;
    solution = vector<int>(masize, -1);
    //cout<<"not this vec3"<<endl;
    for(int k = masize-1; k >= 0; k--){
        if(cury != 0 && grid[cury - 1][curx] == k && canTravel(curx, cury, 3)){
            solution[k] = 1;
            cury = cury-1;
        } else if(curx != 0 && grid[cury][curx - 1] == k && canTravel(curx, cury, 2)){
            solution[k] = 0;
            curx = curx-1;
        } else if(cury != height_-1 && grid[cury+1][curx] == k && canTravel(curx, cury, 1)){
            solution[k] = 3;
            cury = cury+1;
        } else if(curx != width_-1 && grid[cury][curx+1] == k && canTravel(curx, cury, 0)){
            solution[k] = 2;
            curx = curx+1;
        }
        
    }
    

    return solution;


}