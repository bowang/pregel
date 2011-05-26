#ifndef PAGERANK_H
#define PAGERANK_H

#include "Pregel.h"

class PageRankVertex : public Vertex<double, double, double> {
public:
    PageRankVertex (double value, Master<double, double, double>* master) : Vertex<double, double, double>(value, master){};
    virtual void compute (MessageIterator<double>* msgs);
};

#endif
