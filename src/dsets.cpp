/* Your code here! */

#include "dsets.h"
#include <iostream>

// MOST RECENT VERSION

using namespace std;

DisjointSets::DisjointSets(){
    largest_tree = 1;
}

void DisjointSets::addelements(int num){
    // creates num unconnected root nodes at the end of the vector
    for(int i = 0; i < num; i++){
        sets.push_back(-1);
        //std::cout << "addelems isnt segfaulting" << std::endl;
    }

}
int DisjointSets::find(int elem) const{
    // returns index of root of up-tree parameter element is in
    /*if(elem < 0){
        std::cout << "find neg case isnt segfaulting" << std::endl;
        return -1;
    }*/
    if(sets[elem] >= 0){
        //std::cout << "find nonroot case isnt segfaulting" << std::endl;
        return find(sets[elem]);
    } else {
        //std::cout << "find root case isnt segfaulting" << std::endl;
        return elem;
    }
}
void DisjointSets::setunion(int a, int b){
    // union-by-size
    // smaller set (in terms of # of nodes) should point at larger
    int roota = find(a);
    int rootb = find(b);
    /*if(roota == rootb){
        return;
    }*/
    int newSize = sets[roota] + sets[rootb];
    //std::cout << "setunion 1 isnt segfaulting" << std::endl;
    if(sets[roota] <= sets[rootb]){
        sets[rootb] = roota;
        sets[roota] = newSize;
        //std::cout << "setunion2 isnt segfaulting" << std::endl;

    } else {
        sets[roota] = rootb;
        sets[rootb] = newSize;
        //std::cout << "setunion3 isnt segfaulting" << std::endl;
    }
    if(abs(newSize) > largest_tree){
        largest_tree = abs(newSize);
        //cout<<"LT:" <<largest_tree<<std::endl;
    }
}

void DisjointSets::printset(){
    cout << "[";
    for(int i = 0; i < (int)sets.size(); i++){
        cout << sets[i];
        if(i != (int)sets.size()-1){
            cout<<", ";
        }
    }
    cout<<"]"<<endl;
}


int DisjointSets::size(int elem){
    // return number of nodes in up-tree containing the element
    /*int count = 0;
    for(int i = 0; i < (int)sets.size(); i++){
        if(sets[i] == elem){
            count++;
        }
    }
    return count;*/
    //return sizeHelper(find(elem));
    return (-1 * sets[find(elem)]);
}