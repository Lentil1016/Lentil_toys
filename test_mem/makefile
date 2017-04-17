all : asan norm hover sover

norm :heap-use-after-free.cpp
	g++ -O -g -o $@ $^

asan :heap-use-after-free.cpp
	g++ -O -g -fsanitize=address -o $@ $^

hover :heap-buffer-overflow.cpp
	g++ -O -g -fsanitize=address -o $@ $^

sover :stack-buffer-overflow.cpp
	g++ -O -g -fsanitize=address -o $@ $^
