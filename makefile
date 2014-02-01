# Use gcc compiler

CC = gcc

# Compile-time flags

CFLAGS = -std=c99

http_client: http_client.c
	gcc -Wall -o  putgetmachine  http_client.c

