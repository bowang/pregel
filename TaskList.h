#ifndef TASKLIST_H
#define TASKLIST_H

#include "Common.h"

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Vertex;

enum PartitionHeuristics {SimplePartition, EvenPartition, AdaptivePartition, LocalityPartition};
// SimplePartition   : one vertex per task, high scheduling overhead, lowest load imbalance
// EvenPartition     : divide vertices to processors evenly, low scheduling overhead, high load imbalance
// AdaptivePartition : partition vertices firstly in coarse-grain, then in fine-grain
// LocalityPartition : partition vertices according to locality

/*********************************
 *                               *
 *     Declaration of TaskList   *
 *                               *
 *********************************/
template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class TaskList {
public:
    TaskList(vector< Vertex<VertexValue,EdgeValue,MessageValue>* >* vertexList, int numP);
    ~TaskList();
    void insertTask(int vertexId, int taskId);
    bool removeTask(int vertexId, int taskId);
    void partitionTasks(PartitionHeuristics heuristic);
    set<int>* getTask(int taskId);
    int size() {return taskPartition.size();}
    int partitionSize() {return _partitionSize;}
    int partitionNum()  {return _partitionNum;}

private:
    vector< set<int> > taskPartition;
    void simplePartition();
    void evenPartition();
    void adaptivePartition();
    void localityPartition();
    vector< Vertex<VertexValue, EdgeValue, MessageValue>* > *pVertexList;
    int numProcs;
    int _partitionSize;
    int _partitionNum;
};


/*********************************
 *                               *
 *  Implementation of TaskList   *
 *                               *
 *********************************/

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
TaskList<VertexValue, EdgeValue, MessageValue>::TaskList
  (vector< Vertex<VertexValue,EdgeValue,MessageValue>* >* vertexList, int numP)
  : pVertexList(vertexList), numProcs(numP){
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::insertTask(int vertexId, int taskId){
    assert((unsigned)taskId < taskPartition.size());
    taskPartition[taskId].insert(vertexId);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool TaskList<VertexValue, EdgeValue, MessageValue>::removeTask(int vertexId, int taskId){
    assert((unsigned)taskId < taskPartition.size());
    set<int>::iterator it = taskPartition[taskId].find(vertexId);
    if(it != taskPartition[taskId].end()){
        taskPartition[taskId].erase(it);
        return true;
    }
    else{
        return false;
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::partitionTasks(PartitionHeuristics heuristic){
    switch (heuristic) {
        case SimplePartition   : PRINTF("[TaskList] using SimplePartition\n");   simplePartition();   break;
        case EvenPartition     : PRINTF("[TaskList] using EvenPartition\n");     evenPartition();     break;
        case AdaptivePartition : PRINTF("[TaskList] using AdaptivePartition\n"); adaptivePartition(); break;
        case LocalityPartition : PRINTF("[TaskList] using LocalityPartition\n"); localityPartition(); break;
        default: printf("[TaskList] unknown partition heuristic\n"); exit(1);
    };
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::simplePartition(){
    _partitionSize = 1;
    _partitionNum  = pVertexList->size();
    taskPartition.resize(_partitionNum);
    for(unsigned i = 0; i < pVertexList->size(); i++){
        taskPartition[i].insert(i);
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::evenPartition(){
    _partitionSize = pVertexList->size()/numProcs + ((pVertexList->size()%numProcs==0)?0:1);
    _partitionNum  = numProcs;
    taskPartition.resize(_partitionNum);
    for(unsigned i = 0; i < pVertexList->size(); i++){
        taskPartition[i/_partitionSize].insert(i);
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::adaptivePartition(){
    _partitionNum  = 16 * numProcs;
    _partitionSize = pVertexList->size()/_partitionNum + (pVertexList->size()%_partitionNum==0)?0:1;
    taskPartition.resize(_partitionNum);
    
    int i;
    int index = 0;
    int granularity = pVertexList->size()/(2*2*numProcs);
    for(i = 0; i < 2*numProcs; i++){
        for(int j = 0; j < granularity; j++){
            taskPartition[i].insert(index++);
        }
    }
    
    granularity = pVertexList->size()/(4*2*numProcs);
    for(i = 2*numProcs; i < 4*numProcs; i++){
        for(int j = 0; j < granularity; j++){
            taskPartition[i].insert(index++);
        }
    }
    
    granularity = pVertexList->size()/(4*4*numProcs);
    for(i = 4*numProcs; i < 8*numProcs-1; i++){
        for(int j = 0; j < granularity; j++){
            taskPartition[i].insert(index++);
        }
    }
    for(int j = 0; j < granularity; j++){
        if(index < pVertexList->size())
            taskPartition[i].insert(index++);
        else
            break;
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::localityPartition(){
    printf("[TaskList] locality partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
set<int>* TaskList<VertexValue, EdgeValue, MessageValue>::getTask(int taskId){
    return &taskPartition[taskId];
}

#endif
