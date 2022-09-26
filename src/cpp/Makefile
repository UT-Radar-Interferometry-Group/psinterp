CC=g++ -std=c++11 -Wall
PYTHON_LIB=`python3-config --includes` 
PYTHON_EXT=`python3-config --extension-suffix`

default: All

sario.o: sario.hpp sario.cpp
	$(CC) -c sario.cpp

All: sario.o psinterp.cpp
	$(CC) -fopenmp -O3 -shared -fPIC $(PYTHON_LIB) -I `python3 -m pybind11 --includes` -c psinterp.cpp
	$(CC) -fopenmp -O3 -shared -fPIC $(PYTHON_LIB) -I `python3 -m pybind11 --includes` psinterp.o -o psinterp$(PYTHON_EXT)
clean:
	rm -f *.o psinterp$(PYTHON_EXT)
