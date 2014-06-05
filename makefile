CXX = clang++
FLAGS = -std=c++11 -stdlib=libc++
CPPTEST = -I/usr/local/include
CPPUNIT = -I/usr/local/Cellar
CFILES = testing.cpp

pract: *.cpp
	$(CXX) $(FLAGS) $(CFILES) -o pract $(CPPTEST) $(CPPUNIT)

run:
	./pract
clean:
	rm pract