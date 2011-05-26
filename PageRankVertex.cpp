#include "Common.h"
#include "PageRankVertex.h"

void PageRankVertex::compute (MessageIterator<double>* msgs) {
    if (superstep() >= 1) {
        double sum = 0; 
        for (; !(msgs->done()); msgs->next())
            sum += msgs->getValue();
        *mutableValue() = 0.15 / numVertices() + 0.85 * sum;
    }

    if (superstep() < 30) {
        const int n = getOutEdgeIterator().size();
        sendMessageToAllNeighbors (getValue() / n);
    } else {
        voteToHalt();
    }
}
