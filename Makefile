# Rui Daniel, Afonso Alemao 5-04-2022
#
# Compile program: make
# To delete .o and exe: make clean

#   Compiler flags

CC = gcc
CFLAGS = -Wall -ggdb3
#   Sources
SOURCES = gofish.c 

#   Objects  ('make' automatically compiles .c to .o)
OBJECTS = gofish.o

gofish: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

gofish.o: gofish.c 

clean:
	rm *.o gofish
