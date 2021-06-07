ALL:microcluster.o clustream.o Clutest

microcluster.o:microcluster.cpp microcluster.h
	g++ -O3  microcluster.cpp -c -o microcluster.o -lrt
clustream.o:Clustream.cpp CluStream.h
	g++ -O3  Clustream.cpp -c -o clustream.o -lrt
Clutest:clustream.o microcluster.o main.cpp
	g++ -O3  -g --std=c++17 clustream.o microcluster.o main.cpp -o Clutest -lrt

clean:
	rm -rf clustream.o microcluster.o Clutest