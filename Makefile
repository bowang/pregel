CC     = g++
CFLAGS = -pthread -O4 # -Wall
DFLAGS = -DPAGERANK -DDPRINT
OBJS   = Message.o Edge.o Vertex.o TaskList.o Master.o PageRankVertex.o PageRank.o Thread.o Worker.o
BINS   = pregel pagerank_datagen

all: $(BINS)

pregel: $(OBJS) Pregel.h Pregel.cpp
	$(CC) -o $@ $^ $(DFLAGS) $(CFLAGS)

pagerank_datagen: PageRankDataGen.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(BINS)
