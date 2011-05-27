#ifndef EDGE_H
#define EDGE_H

#include "Common.h"


/*********************************
 *                               *
 *     Declaration of Edge       *
 *                               *
 *********************************/

template <typename EdgeValue>
class Edge {
  public:
    Edge(int _dest, EdgeValue _value):dest(_dest), value(_value) {}
    int getDest();
    const EdgeValue& getValue();
    EdgeValue* mutableValue();
    
  private:
    int dest;
    EdgeValue value;
};


/*********************************
 *                               *
 *  Declaration of EdgeIterator  *
 *                               *
 *********************************/

template <typename EdgeValue>
class EdgeIterator {
  public: 
    EdgeIterator() {}
    void set(vector<Edge<EdgeValue>* >& edges);
    bool done();
    void next();
    Edge<EdgeValue>* dest();
    EdgeValue getValue();
    int size();
  private:
    typename vector<Edge<EdgeValue>* >::iterator itr;
    vector<Edge<EdgeValue>* >* edges;
};

/*********************************
 *                               *
 *    Implementation of Edge     *
 *                               *
 *********************************/

template <typename EdgeValue>
const EdgeValue& Edge<EdgeValue>::getValue() {
    return value;
}

template <typename EdgeValue>
EdgeValue* Edge<EdgeValue>::mutableValue() {
    return &value;
}

template <typename EdgeValue>
int Edge<EdgeValue>::getDest() {
    return dest;
}

/**********************************
 *                                *
 * Implementation of EdgeIterator *
 *                                *
 **********************************/

template <typename EdgeValue>
void EdgeIterator<EdgeValue>::set(vector<Edge<EdgeValue>* >& edges) {
    this->edges = &edges;
    itr = edges.begin();
}

template <typename EdgeValue>
bool EdgeIterator<EdgeValue>::done() {
    return (!(itr < edges->end()));
}

template <typename EdgeValue>
void EdgeIterator<EdgeValue>::next() {
    itr++;
    return;
}

template <typename EdgeValue>
Edge<EdgeValue>* EdgeIterator<EdgeValue>::dest() {
    return (*itr);
}

template <typename EdgeValue>
EdgeValue EdgeIterator<EdgeValue>::getValue() {
    return (*itr)->getValue();
}

template <typename EdgeValue>
int EdgeIterator<EdgeValue>::size() {
    return edges->size();
}


#endif
