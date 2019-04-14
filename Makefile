main: alloc prov-rep


alloc: alloc.cpp
	c++ alloc.cpp -o alloc

prov-rep: prov-rep.cpp
	c++ prov-rep.cpp -o prov-rep
