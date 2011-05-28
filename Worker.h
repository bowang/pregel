#ifndef WORKER_H
#define WORKER_H

#include "Thread.h"
#include "Message.h"

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Master;

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
class Worker : public Thread {
public:
    Worker(const int _threadId, Master<VertexValue, EdgeValue, MessageValue>* _master)
          : threadId(_threadId), master(_master) {}
    virtual void* run();
    
private:
    int threadId;
    Master<VertexValue, EdgeValue, MessageValue>* master;

};

template <typename VertexValue,
          typename EdgeValue,
          typename MessageValue>
void* Worker<VertexValue, EdgeValue, MessageValue>::run() {

    PRINTF("[Worker-%d] begin execution\n", threadId);
    #ifdef DPRINT
      FILE* f;
      if(threadId == 0)
        f = fopen ("tmp", "wt");
    #endif
  
    // while termination requirements are not met
    while(true){
        if(master->_haltVoters.concensus(master->_superstep)){
            break;
        }
        PRINTF("[Worker-%d] superstep %d begin\n", threadId, master->superstep());
        bool finished = false;
        // while tasks of a certain superstep are not finished yet
        while(finished==false){
            // get a new task id
            int myTaskId = -1;
            pthread_mutex_lock(&(master->_taskMutex));
            if(master->_curtTaskId < master->_taskList->size()){
                myTaskId = master->_curtTaskId;
                master->_curtTaskId++;
            }
            else{
                finished = true;
            }
            pthread_mutex_unlock(&(master->_taskMutex));

            if(!finished){
                PRINTF("[Worker-%d] assigned task[%d]\n", threadId, myTaskId);
                // get a new task
                set<int> * task = master->_taskList->getTask(myTaskId);
                // process graph
                for(set<int>::iterator itr = task->begin(); itr != task->end(); itr++) {
                    MessageIterator<MessageValue> *msgItr = new MessageIterator<MessageValue> ((*(master->nextMsgList))[*itr]);
                    if(master->vertexActive(*itr)) {
                        (*(master->_vertexList))[*itr]->compute(msgItr);
                    }
                }
            }
        }

        // sync for state re-initialization
        // PRINTF("[Worker-%d] arrived at barrier1\n", threadId);
        pthread_barrier_wait(&(master->_barrier1));
        
        if(threadId == 0){
            FPRINTF (f, "[vertex values] ");
            for (int i = 0; i < master->numVertices(); i++) {
                #ifdef PAGERANK
                FPRINTF (f, "%.6f\t", master->_vertexList->at(i)->getValue());
                #endif
                #ifdef SHORTESTPATH
                FPRINTF (f, "%d\t", master->_vertexList->at(i)->getValue());
                #endif
                #ifdef BIPARTITEMATCHING
                FPRINTF (f, "%d\t", master->_vertexList->at(i)->getValue().second);
                #endif
            }
            FPRINTF (f, "\n");

            // reset states for next superstep
            master->switchMessagelist();
            master->_curtTaskId = 0;

            // TODO: add rescheduling method here, i.e., re-initialize taskList
            
        }
        
        // sync for beginning next superstep
        // PRINTF("[Worker-%d] arrived at barrier2\n", threadId);
        pthread_barrier_wait(&(master->_barrier2));
        
        int i = threadId*master->evenPartitionSize();
        int end = (threadId+1)*master->evenPartitionSize();
        if(end > master->numVertices()) end = master->numVertices();
        for(; i < end; ++i) {
            if(!master->vertexActive(i) 
               && !(*(master->nextMsgList))[i].empty()
               && (*(master->_vertexList))[i]!=NULL ) {
                master->voteToActive(i);
            }
        }
        pthread_barrier_wait(&(master->_barrier3));
    }
    #ifdef DPRINT
      if(threadId == 0)
        fclose (f);
    #endif
    
    return NULL;
}

#endif
