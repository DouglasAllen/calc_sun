SHELL = /bin/sh

# V=0 quiet, V=1 verbose.  other values don't work.
V = 0
Q1 = $(V:1=)
Q = $(Q1:0=@)
ECHO1 = $(V:1=@:)
all: sunriset
sunriset: sunriset.c
	gcc -o sunriset sunriset.c -lm
