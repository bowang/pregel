#ifndef VOTES_H
#define VOTES_H

#include "Common.h"

class Votes {
public:
    Votes () {
        votes.clear();
        positionMap.clear();
    }

    void addVoter(int id) {
        votes.push_back(false);
        positionMap.insert(pair<int, int>(id, votes.size()));
    }

    void removeVoter(int id) {
        votes[positionMap[id]] = true;
        return;
    }

    bool concensus() {
        for (vector<bool>::iterator itr = votes.begin(); itr < votes.end(); itr++) {
            if (!(*itr))
                return false;
        }
        return true;
    }

    void vote(int id) {
        votes[positionMap[id]] = true;
        return;
    }

private:
    vector<bool> votes;
    map<int, int> positionMap;
};


#endif
