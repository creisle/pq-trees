# PQTree Project
# this helped: http://www.cs.northwestern.edu/academics/courses/211/html/make.html

SRCS = src/Leaf.cpp src/Node.cpp src/PQnode.cpp src/PQTree.cpp 
TSRC = src/main.cpp
XSRC = src/Test.cpp
HDRS = include/Leaf.h include/Node.h include/PQnode.h include/PQTree.h
PROJ = pract

CXX = clang++
OBJS = $(SRCS:.cpp=.o)
APP = $(PROJ).exe
CFLAGS = -g -Wall -std=c++11 -stdlib=libc++
INCL = -isystem/usr/local/include
INCLCHK = -I/usr/local/include -I/Users/creisle/GitHub/pq-trees/src/
LDFLAGS = -L/usr/local/lib -L/Users/creisle/GitHub/pq-trees/
LIBS = -lcppunit -ldl -lPQTree

all: $(APP)

#compile into an exe and add the test class here
$(APP): $(OBJS) $(TSRC) lib
	$(CXX) $(CFLAGS) $(INCL) $(TSRC) $(LDFLAGS) $(OBJS) -o $(APP) $(LIBS)

#compile the custom files but not the test class since it needs these oject files
%.o: %.cpp $(HDRS)
	$(CXX) -c  $(CFLAGS) $(INCL) $< -o $@

#make a shared library
lib: $(OBJS)
	ar -cvq libPQTree.a $(OBJS)

#clean up object and exe files
clean:
	rm -f src/*.o $(APP) *.so *.dylib *.a *.plist

#run the program....fairly self explanatory
test:
	./$(APP)

check: $(SRCS)
	cppcheck --enable=all --library=windows.cfg -v $(INCLCHK) $(SRCS) 2> err.txt

