CC     = g++
CFLAGS = -lpthread -O4 # -Wall
DFLAGS = -DSHORTESTPATH -DDPRINT
APPS   = PageRankVertex.o PageRank.o ShortestPathVertex.o ShortestPath.o
OBJS   = Message.o Edge.o Vertex.o TaskList.o Master.o Thread.o Worker.o Timer.o Pregel.o
OBJS  += $(APPS)
BINS   = pregel pagerank_datagen shortestpath_datagen

all: $(BINS)

pregel: $(OBJS)
	$(CC) -o $@ $^ $(DFLAGS) $(CFLAGS)

pagerank_datagen: PageRankDataGen.o
	$(CC) -o $@ $^ $(CFLAGS)

shortestpath_datagen: ShortestPathDataGen.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(DFLAGS)

clean:
	rm -f $(OBJS) $(BINS)
