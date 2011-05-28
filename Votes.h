#ifndef VOTES_H
#define VOTES_H

#include "Common.h"

class Votes {
public:
    Votes () {
        votes.clear();
        positionMap.clear();
        superstepCache = -1;
        concensusCache = false;
        pthread_mutex_init(&concensusMutex, NULL);
    }

    void addVoter(int id) {
        positionMap.insert(pair<int, int>(id, votes.size()));
        votes.push_back(false);
    }

    void removeVoter(int id) {
        votes[positionMap[id]] = true;
    }

    bool concensus() {
        for (vector<bool>::iterator itr = votes.begin(); itr < votes.end(); itr++)
            if (!(*itr)) return false;
        return true;
    }

    bool concensus(int superstep) {
        pthread_mutex_lock(&concensusMutex);
        if(superstep != superstepCache) {
            concensusCache = concensus();
            superstepCache = superstep;
        }
        pthread_mutex_unlock(&concensusMutex);
        return concensusCache;
    }
    
    void vote(const int& id) {
        votes[positionMap[id]] = true;
    }
    
    void unvote(const int& id) {
        votes[positionMap[id]] = false;
    }

    bool vertexActive(const int& id) {
        return !(votes[positionMap[id]]);
    }
    
private:
    vector<bool> votes;
    map<int, int> positionMap;
    int superstepCache;
    bool concensusCache;
    pthread_mutex_t concensusMutex;
};


#endif
