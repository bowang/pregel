#include "Common.h"
#include "BipartiteMatchingVertex.h"

void BipartiteMatchingVertex::compute (MessageIterator<pair<int, bool> >* msgs) {
    int phase = superstep()%4;
    switch(phase) {
        case 0: {
            if (getValue().first == LEFT && getValue().second == NOT_MATCHED) {
                sendMessageToAllNeighbors (make_pair(vertex_id(), false));
            }
            voteToHalt();
            break;
        }
        case 1: {
            if (getValue().first == RIGHT && getValue().second == NOT_MATCHED) {
                vector <int> requests;
                for (; !msgs->done(); msgs->next())
                    requests.push_back(msgs->getValue().first);
                if (requests.size() != 0) {
                    srand(0);
                    int granted = rand()%requests.size();
                    for (int i = 0; i < requests.size(); i++)
                        sendMessageTo(requests.at(i), make_pair(vertex_id(), i == granted));
                }
            }
            voteToHalt();
            break;
        }
        case 2: {
            if (getValue().first == LEFT && getValue().second == NOT_MATCHED) {
                vector <int> grants;
                for (; !msgs->done(); msgs->next()) {
                    if (msgs->getValue().second)
                        grants.push_back(msgs->getValue().first);
                }
                if (grants.size() != 0) {
                    srand(0);
                    int accepted = rand()%grants.size();
                    (*mutableValue()).second = grants.at(accepted);
                    sendMessageTo(grants.at(accepted), make_pair(vertex_id(), true));
                }
            }
            break;
        }
        case 3: {
            if (getValue().first == RIGHT && getValue().second == NOT_MATCHED) {
                if (!msgs->empty() && msgs->getValue().second)
                    (*mutableValue()).second = msgs->getValue().first;
            }
            voteToHalt();
            break;
        }
        default:
            break;
    }
}
