CXX = clang++
FLAGS = -std=c++11 -stdlib=libc++
pract: *.cpp
	$(CXX) $(FLAGS) *.cpp -o pract
run:
	./pract
clean:
	rm pract