#ifndef MASTER_H
#define MASTER_H

#include "Common.h"
#include "Vertex.h"
#include "TaskList.h"
#include "Votes.h"
#include "Worker.h"

/*********************************
 *                               *
 *    Declaration of Master      *
 *                               *
 *********************************/
template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Master {
public:
    Master(const int& numProcs, const PartitionHeuristics& partition);
    ~Master(){};
    void initialTasks();
    void run();
    int superstep() const {return _superstep;}
    int numVertices() const {return _numVertices;}
    int evenPartitionSize() const {return _evenPartitionSize;}
    bool receiveMessage(const int& dest_vertex, const MessageValue& message);
    void voteToHalt(const int& id) { _haltVoters.vote(id); }
    void voteToActive(const int& id) { _haltVoters.unvote(id); }
    void addVertex(Vertex<VertexValue, EdgeValue, MessageValue> *vertex);
    void removeVertex(const int& id);
    bool vertexActive(const int& id);
    
    friend class Worker<VertexValue, EdgeValue, MessageValue>;

private:
    vector< Vertex<VertexValue, EdgeValue, MessageValue>* > *_vertexList;
    TaskList<VertexValue, EdgeValue, MessageValue> *_taskList;
    vector< MessageList<MessageValue> > msgList0;
    vector< MessageList<MessageValue> > msgList1;
    vector< MessageList<MessageValue> > *curtMsgList;
    vector< MessageList<MessageValue> > *nextMsgList;
    int  _numProcs;
    int  _numVertices;
    int  _superstep;
    int  _curtTaskId;
    int  _evenPartitionSize;
    Votes _haltVoters;
    vector< Worker<VertexValue, EdgeValue, MessageValue>* > _workers;
    pthread_mutex_t _taskMutex;
    pthread_barrier_t _barrier1;
    pthread_barrier_t _barrier2;
    pthread_barrier_t _barrier3;
    PartitionHeuristics  _partitionHeuristic;

    void switchMessagelist();
    void removeVertexFromAdaptivePartition(const int& id);
    void removeVertexFromLocalityPartition(const int& id);
};

/*********************************
 *                               *
 *  Implementation of Master     *
 *                               *
 *********************************/

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
Master<VertexValue, EdgeValue, MessageValue>::Master(const int& numProcs, const PartitionHeuristics& partition)
  : _numProcs(numProcs), _partitionHeuristic(partition) {
    _numVertices = 0;
    _vertexList = new vector < Vertex<VertexValue, EdgeValue, MessageValue>* >;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::initialTasks(){
    _superstep = 0;
    _curtTaskId = 0;
    msgList0.clear();
    msgList1.clear();
    msgList0.resize(_numVertices);
    msgList1.resize(_numVertices);
    curtMsgList = &msgList0;
    nextMsgList = &msgList1;
    _taskList = new TaskList<VertexValue, EdgeValue, MessageValue>(_vertexList, _numProcs);
    _taskList->partitionTasks(_partitionHeuristic);
    PRINTF("[Master] #vertices = %d, #partitions = %d\n", _numVertices, _taskList->size());

}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::run(){    

    PRINTF("[Master] graph processing started\n");    
    
    pthread_mutex_init(&_taskMutex, NULL);
    pthread_barrier_init(&_barrier1, NULL, _numProcs);
    pthread_barrier_init(&_barrier2, NULL, _numProcs);
    pthread_barrier_init(&_barrier3, NULL, _numProcs);

    _evenPartitionSize = _numVertices/_numProcs + ((_numVertices%_numProcs==0)?0:1);
    
    PRINTF("[Master] creating %d threads\n", _numProcs);
    for(int i = 0; i < _numProcs; i++){
        _workers.push_back(new Worker<VertexValue, EdgeValue, MessageValue>(i, this));
        _workers[i]->start();
    }
    
    for(int i = 0; i < _numProcs; i++){
        _workers[i]->join();
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::switchMessagelist() {
    printf("[Master] superstep %d finished\n\n", _superstep);
    _superstep++;
    for(unsigned i = 0; i < nextMsgList->size(); i++)
        (*nextMsgList)[i].clear();
    vector< MessageList<MessageValue> > *tmp = curtMsgList;
    curtMsgList = nextMsgList;
    nextMsgList = tmp;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::removeVertex(const int& id) {
    assert(id < _numVertices);

    // remove from haltVoter
    _haltVoters.removeVoter(id);
    
    // remove messageList
    msgList0[id].clear();
    msgList1[id].clear();
    
    // remove its outEdges
    (*_vertexList)[id]->removeOutEdges();
    delete (*_vertexList)[id];
    (*_vertexList)[id] = NULL;
    
    // remove from taskList
    switch(_partitionHeuristic) {
        case SimplePartition   : _taskList->removeTask(id, id); break;
        case EvenPartition     : _taskList->removeTask(id, id/_taskList->partitionSize()); break;
        case AdaptivePartition : removeVertexFromAdaptivePartition(id); break;
        case LocalityPartition : removeVertexFromLocalityPartition(id); break;
        default: printf("[Master] unknown partition heuristic\n"); exit(1);
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void removeVertexFromAdaptivePartition(const int& id) {
    printf("[Master] adaptive partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void removeVertexFromLocalityPartition(const int& id) {
    printf("[Master] locality partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::addVertex(Vertex<VertexValue, EdgeValue, MessageValue> *vertex) {
    _vertexList->push_back(vertex);
    _numVertices = _vertexList->size();
    vertex->master = this;
    _haltVoters.addVoter(vertex->id);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Master<VertexValue, EdgeValue, MessageValue>::receiveMessage(const int& dest_vertex, const MessageValue& message) {
    // assert(dest_vertex < _numVertices);
    
    if((*_vertexList)[dest_vertex]!=NULL){
        (*curtMsgList)[dest_vertex].addMessage(message);
        return true;
    }
    else{
        return false;
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool Master<VertexValue, EdgeValue, MessageValue>::vertexActive(const int& id) {
    return _haltVoters.vertexActive(id);
}

#endif
