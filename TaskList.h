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

#ifdef PRINTF
  #define DPRINT_FLAG
  #undef  PRINTF
  #define PRINTF(...)
#endif

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::adaptivePartition(){
    int smallPartitionNum = 16 * numProcs;
    int numVertices = pVertexList->size();
    _partitionNum  = 8 * numProcs;
    _partitionSize = -1;
    taskPartition.resize(_partitionNum);
    
    int i, numRemaining;
    int index = 0;
    int granularity = numVertices/(2*2*numProcs) + ((numVertices%(2*2*numProcs)==0)?0:1);
    for(i = 0; i < 2*numProcs; i++){
        for(int j = 0; j < granularity; j++){
            taskPartition[i].insert(index++);
            PRINTF("%d ", index-1);
        }
        PRINTF("\n");
    }
    PRINTF("-----\n");
    
    numRemaining = numVertices - index;
    granularity = numRemaining/(2*2*numProcs) + ((numRemaining%(2*2*numProcs)==0)?0:1);
    for(i = 2*numProcs; i < 4*numProcs; i++){
        for(int j = 0; j < granularity; j++){
            taskPartition[i].insert(index++);
            PRINTF("%d ", index-1);
        }
        PRINTF("\n");
    }
    PRINTF("-----\n");
    
    numRemaining = numVertices - index;
    granularity = numRemaining/(4*numProcs) + ((numRemaining%(4*numProcs)==0)?0:1);
    for(i = 4*numProcs; i < 8*numProcs - 1; i++){
        for(int j = 0; j < granularity; j++){
            if(index < numVertices) {
                taskPartition[i].insert(index++);
                PRINTF("%d ", index-1);
            }
        }
        PRINTF("\n");
    }
    PRINTF("-----\n");
    
    while(index < numVertices){
        taskPartition[i].insert(index++);
        PRINTF("%d ", index-1);
    }
    PRINTF("\n");

    for(int k = 0; k < _partitionNum; k++){
        if(taskPartition[k].empty()){
            _partitionNum = k;
            PRINTF("_partitionNum = %d\n", _partitionNum);
            typename vector< set<int> >::iterator itr1 = taskPartition.begin() + k;
            typename vector< set<int> >::iterator itr2 = taskPartition.end() - 1;
            for(;itr2 >= itr1; itr2--){
                taskPartition.erase(itr2);
                PRINTF("erase taskPartition[%d]\n", itr2 - taskPartition.begin());
            }
            break;
        }
    }    
    PRINTF("index = %d, numVertices = %d\n", index, numVertices);
}

#ifdef DPRINT_FLAG
  #undef  DPRINT_FLAG
  #undef  PRINTF
  #define PRINTF(...) printf(__VA_ARGS__)
#endif

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
