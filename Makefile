all: run-test

run-test: test
	./test

test: test.c mini-ppm-info.c mini-ppm-info.h
	cc -ggdb3 -o test test.c mini-ppm-info.c

vi:
	vim Makefile test.c mini-ppm-info.c mini-ppm-info.h
