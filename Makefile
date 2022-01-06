all: ws

ws: whitespace-skip run-ws

run-ws:
	./whitespace-skip

whitespace-skip: whitespace-skip.c
	cc -ggdb3 -o whitespace-skip whitespace-skip.c
