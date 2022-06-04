all: run-test

run-test: test
	./test

test: tests/test.c src/mini-ppm-info.c src/mini-ppm-info.h
	cc -Wall -ggdb3 -o test tests/test.c src/mini-ppm-info.cpp

vi:
	vim README.md \
		library.properties \
		Makefile \
		tests/test.c \
		src/mini-ppm-info.cpp \
		src/mini-ppm-info.h
