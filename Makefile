all: run-test

run-test: test
	./test

test: tests/test.c mini-ppm-info.c mini-ppm-info.h
	cc -Wall -ggdb3 -o test tests/test.c mini-ppm-info.c

vi:
	vim README.md Makefile test.c mini-ppm-info.c mini-ppm-info.h
