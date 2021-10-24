#this file builds and runs the files listed in SOURCE
SOURCE = src/life.cpp
EXEC = build/life

#Compiler and Directives
CPP = g++
LFLAGS = -lSDL2 -lSDL2main #link the SDL libraries
CFLAGS = $(SOURCE) --std=c++20 -Wall -pedantic $(LFLAGS) -o $(EXEC) #latest standard, show all warnings (strict)

#Default rule
all: $(EXEC)

$(EXEC):
	echo "compiling $(SOURCE)"
	$(CPP) $(CFLAGS) 

#run the executable
run: $(EXEC)
	echo "running $(EXEC)"
	./$(EXEC)

#rule for cleaning execs
clean:
	-rm -f $(EXEC)