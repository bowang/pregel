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
    void insertTask(int vertexId, int partitionId);
    bool removeTask(int vertexId, int partitionId);
    void partitionTasks(PartitionHeuristics heuristic);
    set<int>* getTaskPartition(int partitionId);
    int size();

private:
    vector< set<int> > taskPartition;
    void simplePartition();
    void evenPartition();
    void adaptivePartition();
    void localityPartition();
    void dynamicPartition();
    vector< Vertex<VertexValue, EdgeValue, MessageValue>* > *pVertexList;
    int numProcs;
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
void TaskList<VertexValue, EdgeValue, MessageValue>::insertTask(int vertexId, int partitionId){
    assert(partitionId < taskPartition.size());
    taskPartition[partitionId].insert(vertexId);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
bool TaskList<VertexValue, EdgeValue, MessageValue>::removeTask(int vertexId, int partitionId){
    assert(partitionId < taskPartition.size());
    set<int>::iterator it = taskPartition[partitionId].find(vertexId);
    if(it != taskPartition[partitionId].end()){
        taskPartition[partitionId].erase(it);
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
        case SimplePartition   : simplePartition();   break;
        case EvenPartition     : evenPartition();     break;
        case AdaptivePartition : adaptivePartition(); break;
        case LocalityPartition : localityPartition(); break;
        default: printf("[error] unknown partition heuristic\n"); exit(1);
    };
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
int TaskList<VertexValue, EdgeValue, MessageValue>::size(){
    return taskPartition.size();
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::simplePartition(){
    taskPartition.resize(pVertexList->size());
    for(unsigned i = 0; i < pVertexList->size(); i++){
        taskPartition[i].insert(i);
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::evenPartition(){
    int partitionSize = pVertexList->size()/numProcs + (pVertexList->size()%numProcs==0)?0:1;
    int partitionNum  = numProcs;
    taskPartition.resize(numProcs);

    for(unsigned i = 0; i < pVertexList->size(); i++){
        taskPartition[i/partitionSize].insert(i);
    }
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::adaptivePartition(){
    printf("[error] adaptive partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::dynamicPartition(){
    printf("[error] dynamic partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void TaskList<VertexValue, EdgeValue, MessageValue>::localityPartition(){
    printf("[error] locality partition is not supported yet\n");
    exit(1);
}

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
set<int>* TaskList<VertexValue, EdgeValue, MessageValue>::getTaskPartition(int partitionId){
    return &taskPartition[partitionId];
}

#endif
