#ifndef BIPARTITEMATCHING_H
#define BIPARTITEMATCHING_H

#include "Pregel.h"

#define LEFT false
#define RIGHT true

#define NOT_MATCHED -1

class BipartiteMatchingVertex : public Vertex<pair<bool, int>, int, pair<int, bool> > {
public:
    BipartiteMatchingVertex (pair<bool, int> value, Master<pair<bool, int>, int, pair<int, bool> >* master) : Vertex<pair<bool, int>, int, pair<int, bool> > (value, master) {};
    virtual void compute (MessageIterator<pair<int, bool> >* msgs);
};

#endif
