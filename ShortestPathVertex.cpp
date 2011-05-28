#include "Common.h"
#include "ShortestPathVertex.h"
#include <algorithm>

void ShortestPathVertex::compute (MessageIterator<int>* msgs) {
    int mindist =  (superstep() == 0 && vertex_id() == 0) ? 0 : getValue();

    for (; !msgs->done(); msgs->next())
        mindist = min (mindist, msgs->getValue());

    if (mindist < getValue()) {
        *mutableValue() = mindist;
        EdgeIterator<int> iter = getOutEdgeIterator();
        for (; !iter.done(); iter.next())
            sendMessageTo(iter.dest()->getDest(), mindist + iter.dest()->getValue());
    }

    voteToHalt();
}
