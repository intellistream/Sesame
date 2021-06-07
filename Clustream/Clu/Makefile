ALL:kernel.o clustream.o Clutest

kernel.o:kernel.cpp kernel.h
	g++ -O3  kernel.cpp -c -o kernel.o -lrt
clustream.o:Clustream.cpp CluStream.h
	g++ -O3  Clustream.cpp -c -o clustream.o -lrt
Clutest:clustream.o kernel.o main.cpp
	g++ -O3  -g --std=c++17 clustream.o kernel.o main.cpp -o Clutest -lrt

clean:
	rm -rf clustream.o kernel.o Clutest