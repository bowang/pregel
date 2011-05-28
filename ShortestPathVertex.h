#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "Pregel.h"

class ShortestPathVertex : public Vertex<int, int, int> {
public:
    ShortestPathVertex (int value, Master<int, int, int>* master) : Vertex<int, int, int>(value, master){};
    virtual void compute (MessageIterator<int>* msgs);
};

#endif
