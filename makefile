#this file builds and runs the files listed in SOURCE
SOURCE = hello_world.cpp
EXEC = hello_world

#Compiler and Directives
CPP = g++
CFLAGS = --std=c++20 $(SOURCE) -o $(EXEC) -Wall -pedantic #latest standard, show all warnings (strict)

#Default rule
all: $(EXEC)

$(EXEC):
	echo "compiling $(EXEC)"
	$(CPP) $(CFLAGS)

#run the executable
run: $(EXEC)
	echo "running $(EXEC)"
	./$(EXEC)

#rule for cleaning execs
clean:
	-rm -f $(EXEC)