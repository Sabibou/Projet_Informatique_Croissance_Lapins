CC=gcc
CFLAGS=-c -Wall -Wextra
LDFLAGS=-lm
SOURCES=test.c mersenne_twister.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main
.PHONY : clean
all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)
.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean :
	rm -rf $(OBJETS) $(EXECUTABLE)
