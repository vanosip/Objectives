build:
	gcc main.c Graph.c dijkstra.c MaxFlow.c -o final
clean:
	rm -f final *.o