all: test_run

test_run: test
	@echo "Running ./test"
	./test

test: test.c ../src/mini-ppm-info.cpp
	@echo "Building test.c from .cpp"
	g++ --std=c++11 -o $@ $^

vi:
	vim \
		Makefile test.c ../src/mini-ppm-info.cpp ../src/mini-ppm-info.h
