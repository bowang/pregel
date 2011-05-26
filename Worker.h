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

    // while termination requirements are not met
    while(!master->_haltVoters.concensus()){
        PRINTF("[Worker-%d] superstep %d begin\n", threadId, master->superstep());
        master->_finished = false;
        // while tasks of a certain superstep are not finished yet
        while(master->_finished==false){
            // get a new task id
            int myTaskId = -1;
            pthread_mutex_lock(&(master->_taskMutex));
            if(master->_curtTaskId < master->_taskList->size()){
                myTaskId = master->_curtTaskId;
                master->_curtTaskId++;
            }
            else{
                master->_finished = true;
            }
            pthread_mutex_unlock(&(master->_taskMutex));

            if(!master->_finished){
                PRINTF("[Worker-%d] assigned task[%d]\n", threadId, myTaskId);
                // get a new task
                set<int> * task = master->_taskList->getTask(myTaskId);
                // process graph
                for(set<int>::iterator itr = task->begin(); itr != task->end(); itr++) {
                    MessageIterator<MessageValue> *msgItr = new MessageIterator<MessageValue> ((*(master->nextMsgList))[*itr]);
                    (*(master->_vertexList))[*itr]->compute(msgItr);
                }
            }
        }

        // sync at superstep
        master->_threadReady[threadId] = true;
        if(threadId != 0){
            // [thread 1 ~ n-1]
            PRINTF("[Worker-%d] going to sleep\n", threadId);
            pthread_cond_wait(&(master->_taskSync), NULL);
        }
        else{
            // [thread 0]
            // wait until all other threads are ready
            bool allReady = false;
            while(!allReady){
                allReady = true;
                for(int i = 0; i < master->_numProcs; i++){
                    if(master->_threadReady[i]==false)
                        allReady = false;
                }
            }
            printf("[Worker-%d] superstep %d finish\n", threadId, master->_superstep);
            // reset states for next superstep
            master->switchMessagelist();
            pthread_mutex_lock(&(master->_taskMutex));
            master->_curtTaskId = 0;
            pthread_mutex_unlock(&(master->_taskMutex));
            for(int i = 0; i < master->_numProcs; i++){
                master->_threadReady[i] = false;
            }
            // TODO: add rescheduling method here, i.e., re-initialize taskList
            
            // signal all other threads
            pthread_cond_broadcast(&(master->_taskSync));
        }
    }
    return NULL;
}

#endif
