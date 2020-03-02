##########################
#MakeFile
#Lucas Manker
#COSC 3750 Spring 2020
#Homework #5
#2/25/20
#
#Makefile for hw4
#########################

CC=gcc
CFLAGS=-ggdb -Wall

.PHONY: clean

wycat: wyls.c
	$(CC) $(CFLAGS) wyls.c -o wyls



clean:
	/bin/rm -f wyls
