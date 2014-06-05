CXX = clang++
FLAGS = -std=c++11 -stdlib=libc++

pract: *.cpp
	$(CXX) $(FLAGS) *.cpp -o pract -I/usr/local/include
run:
	./pract
clean:
	rm pract