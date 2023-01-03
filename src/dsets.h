/* Your code here! */
#pragma once

#include <vector>
using namespace std;

// MOST RECENT VERSION

class DisjointSets {
    public:
    DisjointSets();
    void addelements(int num);
    int find(int elem) const;
    void setunion(int a, int b);
    int size(int elem);
    vector<int> sets;
    int largest_tree;
    void printset();

};