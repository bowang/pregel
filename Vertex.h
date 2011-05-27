#ifndef VERTEX_H
#define VERTEX_H

#include "Common.h"
#include "Edge.h"
#include "Message.h"

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Master;

/*********************************
 *                               *
 *     Declaration of Vertex     *
 *                               *
 *********************************/

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Vertex {
  public:
    Vertex(VertexValue _value, Master<VertexValue, EdgeValue, MessageValue>* _master = NULL);
    ~Vertex();
    
    // user defined method
    virtual void compute(MessageIterator<MessageValue>* msgs) = 0;
    
    // methods for users to call
    const int& vertex_id() const;
    int superstep() const;
    const VertexValue& getValue();
    VertexValue* mutableValue();
    void addOutEdge(const int &dest, const EdgeValue &val);
    bool removeOutEdgeFirst(const int &dest);
    bool removeOutEdgeAll(const int &dest);
    void removeOutEdges();
    EdgeIterator<EdgeValue> getOutEdgeIterator();
    bool sendMessageTo(const int& dest_vertex, const MessageValue& message);
    bool sendMessageToAllNeighbors(const MessageValue& message);
    void voteToHalt();
    int  numVertices();
    
    friend class Master<VertexValue, EdgeValue, MessageValue>;

  protected:
    //virtual void combine(const int& dest_vertex, const MessageValue& message) {}
    int id;
    
  private:
    VertexValue value;
    vector<Edge<EdgeValue>*> outEdgeList;
    Master<VertexValue, EdgeValue, MessageValue>* master;
    EdgeIterator<EdgeValue> itr;
};


/*********************************
 *                               *
 *   Implementation of Vertex    *
 *                               *
 *********************************/

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
Vertex<VertexValue, EdgeValue, MessageValue>::Vertex
    (VertexValue _value, Master<VertexValue, EdgeValue, MessageValue>* _master)
    :value(_value), master(_master) {

    id = master->numVertices();
    master->addVertex(this);
    outEdgeList.clear();
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
Vertex<VertexValue, EdgeValue, MessageValue>::~Vertex() {
    master->removeVertex(id);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
const int& Vertex<VertexValue, EdgeValue, MessageValue>::vertex_id() const {
    return id;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
int Vertex<VertexValue, EdgeValue, MessageValue>::superstep() const {
    return master->superstep();
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
const VertexValue& Vertex<VertexValue, EdgeValue, MessageValue>::getValue() {
    return value;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Vertex<VertexValue, EdgeValue, MessageValue>::addOutEdge(const int &dest, const EdgeValue &val) {
    Edge<EdgeValue>* e= new Edge<EdgeValue>(dest, val);
    outEdgeList.push_back(e);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Vertex<VertexValue, EdgeValue, MessageValue>::removeOutEdgeFirst(const int &dest) {
    for(typename vector<Edge<EdgeValue>*>::iterator itr = outEdgeList.begin(); itr != outEdgeList.end(); itr++) {
        if((*itr)->getDest()==dest){
            outEdgeList.erase(itr);
            return true;
        }
    }
    return false;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Vertex<VertexValue, EdgeValue, MessageValue>::removeOutEdgeAll(const int &dest) {
    bool found = false;
    for(typename vector<Edge<EdgeValue>*>::iterator itr = outEdgeList.begin(); itr != outEdgeList.end(); itr++) {
        if((*itr)->getDest()==dest){
            outEdgeList.erase(itr);
            found = true;
        }
    }
    return found;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Vertex<VertexValue, EdgeValue, MessageValue>::removeOutEdges() {
    for(typename vector<Edge<EdgeValue>*>::iterator itr = outEdgeList.begin(); itr != outEdgeList.end(); itr++) {
        delete (*itr);
    }
    outEdgeList.clear();
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
EdgeIterator<EdgeValue> Vertex<VertexValue, EdgeValue, MessageValue>::getOutEdgeIterator() {
    itr.set(outEdgeList);
    return itr;
}


template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
VertexValue* Vertex<VertexValue, EdgeValue, MessageValue>::mutableValue() {
    return (&value);
}


template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
int Vertex<VertexValue, EdgeValue, MessageValue>::numVertices() {
    return master->numVertices();
}



template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Vertex<VertexValue, EdgeValue, MessageValue>::voteToHalt() {
    master->voteToHalt(id);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Vertex<VertexValue, EdgeValue, MessageValue>::sendMessageToAllNeighbors(const MessageValue& message) {
    for (typename vector<Edge<EdgeValue>*>::iterator itr = outEdgeList.begin(); itr != outEdgeList.end(); itr++) {
        int dest_vertex = (*itr)->getDest();
        sendMessageTo(dest_vertex, message);		
    }
    return true;
}


template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Vertex<VertexValue, EdgeValue, MessageValue>::sendMessageTo(const int& dest_vertex, const MessageValue& message) {
    return master->receiveMessage(dest_vertex, message);
}

#endif
