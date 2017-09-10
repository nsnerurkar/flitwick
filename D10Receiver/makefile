# Declaration of variables
CC = g++
CC_FLAGS = -g -w -std=c++11 -fpermissive
LD_FLAGS = -ldl -pthread
 
# File names
EXEC = d10server
SOURCES = $(wildcard *.c*)
OBJECTS = $(SOURCES:.cpp=.o) sqlite3.o rs232.o

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) sqlite3.o nqueue.o d10handler.o rs232.o quicksock.o d10lis.o main.o $(LD_FLAGS) -o $(EXEC) 

 
# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< $(LD_FLAGS) -o $@ 
 
%.o: %.c
	gcc -c $< $(LD_FLAGS) -o $@


# To remove generated files
clean:
	rm -f $(EXEC)
	rm -f *.o
