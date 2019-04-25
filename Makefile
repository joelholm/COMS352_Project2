main: semAndMem alloc prov-rep hw8

alloc: alloc.cpp semAndMem.o
	g++ alloc.cpp -o alloc

prov-rep: prov-rep.cpp
	g++ prov-rep.cpp -o prov-rep

semAndMem: semAndMem.h
	g++ -c semAndMem.h -o semAndMem.o

homework8: hw8.cpp
	g++ hw8.cpp -o hw8

clean:
	rm -f alloc
	rm -f prov-rep
	rm -f *.o
