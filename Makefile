all:
	g++ -std=c++17 -O3 main.cpp -o Exercise
clean:
	rm -f Exercise
	rm output*