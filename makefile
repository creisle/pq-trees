# PQTree Project
# this helped: http://www.cs.northwestern.edu/academics/courses/211/html/make.html

SRCS = Leaf.cpp Node.cpp PQnode.cpp PQTree.cpp 
TSRC = PQTreeTests.cpp
HDRS = Leaf.h Node.h PQnode.h PQTree.h
PROJ = pract

CXX = clang++
OBJS = $(SRCS:.cpp=.o)
APP = $(PROJ).exe
CFLAGS = -g -Wall -std=c++11 -stdlib=libc++ -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LIBS = -lcppunit -ldl

all: $(APP)

#compile into an exe and add the test class here
$(APP): $(OBJS) $(TSRC)
	$(CXX) $(CFLAGS) $(TSRC) $(LDFLAGS) $(OBJS) -o $(APP) $(LIBS)

#compile the custom files but not the test class since it needs these oject files
%.o: %.cpp $(HDRS)
	$(CXX) -c  $(CFLAGS) $< -o $@

#clean up object and exe files
clean:
	rm -f *.o $(APP)

#run the program....fairly self explanatory
run:
	./$(APP)

