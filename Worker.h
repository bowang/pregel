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
    while(true){
        if(master->_haltVoters.concensus()){
            master->_threadReady[threadId] = true;
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
                    (*(master->_vertexList))[*itr]->compute(msgItr);
                }
            }
        }

        // sync at superstep
        if(threadId != 0){
            // [thread 1 ~ n-1]
            PRINTF("[Worker-%d] going to sleep\n", threadId);
            pthread_mutex_lock(&(master->_syncMutex));
            master->_threadReady[threadId] = true;
            pthread_cond_wait(&(master->_taskSync), &(master->_syncMutex));
            PRINTF("[Worker-%d] awake\n", threadId);
            pthread_mutex_unlock(&(master->_syncMutex));
        }
        else{
            // [thread 0]
            // wait until all other threads are ready
            bool allReady = false;
            while(!allReady){
                allReady = true;
                for(int i = 1; i < master->_numProcs; i++){
                    if(master->_threadReady[i]==false)
                        allReady = false;
                }
            }
            printf("[Worker-%d] superstep %d finish\n\n", threadId, master->_superstep);
            // reset states for next superstep
            master->switchMessagelist();
            master->_curtTaskId = 0;
            for(int i = 1; i < master->_numProcs; i++){
                master->_threadReady[i] = false;
            }
            // TODO: add rescheduling method here, i.e., re-initialize taskList
            
            // signal all other threads
            pthread_mutex_lock(&(master->_syncMutex));
            PRINTF("[Worker-%d] pthread_cond_broadcast\n", threadId);
            pthread_cond_broadcast(&(master->_taskSync));
            PRINTF("[Worker-%d] out of mutex\n", threadId);
            pthread_mutex_unlock(&(master->_syncMutex));
        }
    }
    return NULL;
}

#endif
