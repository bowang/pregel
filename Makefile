CC     = g++
CFLAGS = -pthread -O4 # -Wall
DFLAGS = -DPAGERANK # -DDPRINT
APPS   = PageRankVertex.o PageRank.o
OBJS   = Message.o Edge.o Vertex.o TaskList.o Master.o Thread.o Worker.o Timer.o
OBJS  += $(APPS)
BINS   = pregel pagerank_datagen

all: $(BINS)

pregel: $(OBJS) Pregel.h Pregel.cpp
	$(CC) -o $@ $^ $(DFLAGS) $(CFLAGS)

pagerank_datagen: PageRankDataGen.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(BINS)
