all: find_x

CC=gcc

LIBS=-lm
CFLAGS=-Os -pipe -s
DEBUGCFLAGS=-Og -pipe -g

INPUT=find_x.c
INPUT_CONCRETE=find_x.concrete.c
OUTPUT=find_x

RM=/bin/rm

.PHONY: find_x
find_x:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS)

concrete:
	$(CC) $(INPUT_CONCRETE) -o $(OUTPUT) $(LIBS) $(CFLAGS)

debug:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(DEBUGCFLAGS)

clean:
	if [ -e $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
