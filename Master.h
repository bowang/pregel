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
    Master(const int& numProcs);
    ~Master(){};
    void initialTasks();
    void run();
    int superstep() const {return _superstep;};
    int numVertices() const {return _numVertices;};
    bool receiveMessage(const int& dest_vertex, const MessageValue& message);
    void voteToHalt(const int& id) { _haltVoters.vote(id); };
    void addVertex(Vertex<VertexValue, EdgeValue, MessageValue> *vertex);
    void removeVertex(const int& id);
    
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
    Votes _haltVoters;
    vector< Worker<VertexValue, EdgeValue, MessageValue>* > _workers;
    bool *_threadReady;
    pthread_mutex_t _taskMutex;
    pthread_mutex_t _syncMutex;
    pthread_cond_t  _taskSync;
    PartitionHeuristics  _partitionHeuristic;

    void switchMessagelist();

};

/*********************************
 *                               *
 *  Implementation of Master     *
 *                               *
 *********************************/

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
Master<VertexValue, EdgeValue, MessageValue>::Master(const int& numProcs): _numProcs(numProcs){
    _numVertices = 0;
    _partitionHeuristic = SimplePartition;
    _threadReady = new bool[_numProcs];
    _vertexList = new vector < Vertex<VertexValue, EdgeValue, MessageValue>* >;
    _taskList = new TaskList<VertexValue, EdgeValue, MessageValue>(_vertexList, _numProcs);
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
    pthread_cond_init(&_taskSync, NULL);
    pthread_mutex_init(&_syncMutex, NULL);
    _threadReady[0] = true;

    PRINTF("[Master] creating %d threads\n", _numProcs);
    for(int i = 0; i < _numProcs; i++){
        _workers.push_back(new Worker<VertexValue, EdgeValue, MessageValue>(i, this));
        _threadReady[i] = false;
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
    _superstep++;
    for(unsigned i = 0; i < curtMsgList->size(); i++)
        (*curtMsgList)[i].clear();
    vector< MessageList<MessageValue> > *tmp = curtMsgList;
    curtMsgList = nextMsgList;
    nextMsgList = tmp;
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void Master<VertexValue, EdgeValue, MessageValue>::removeVertex(const int& id) {
    _haltVoters.removeVoter(id);
    // TODO: how to remove the vertex from both vertexList and taskList?
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
    (*curtMsgList)[dest_vertex].addMessage(message);
    return true;
}

#endif
