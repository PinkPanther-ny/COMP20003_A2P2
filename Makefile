# Makefile written by NuoyanChen 9/2020
# 2020s2 UNIMELB COMP20003 assignment 2


# Not sure if you markers will do the test manully, so I commented it out
# Also commented out the 'run' and 'show'
# but clean object files remains :)
##########################################################################


CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

VALGRIND = valgrind
VALGRINDFLAG = --leak-check=full --read-var-info=yes --track-origins=yes --tool=memcheck

DEFAULT = map
PART1 = map1
PART2 = map2

SOURCE_FILES = $(shell find . -maxdepth 1 -type f -name '*.c')
HEADER_FILES = $(shell find . -maxdepth 1 -type f -name '*.h')
OBJECT_FILES = $(SOURCE_FILES:.c=.o)
cls_cmd:=tput clear

.PHONY: all build clean run show valgrind

#DATA_FILE = data/small.csv
#DATA_FILE = data/datafile
#DATA_FILE = data/CLUEdata2018_median.csv
DATA_FILE = data/CLUEdata2018_random.csv
#DATA_FILE = data/CLUEdata2018_sortx.csv
#DATA_FILE = data/1.csv
OUTPUT_FILE = data/outputfile
KEYWORD_FILE = data/queryfile

all: build run clean #show

build: $(DEFAULT)

clean:
	rm -f $(OBJECT_FILES) $(PART1) $(PART2) $(DEFAULT)
	#$(cls_cmd)

run: build
	./$(DEFAULT) $(DATA_FILE) $(OUTPUT_FILE) < $(KEYWORD_FILE)

show:
	cat $(OUTPUT_FILE)

valgrind: build clean
	$(cls_cmd)
	$(VALGRIND) $(VALGRINDFLAG) ./$(DEFAULT) $(DATA_FILE) $(OUTPUT_FILE) < $(KEYWORD_FILE)

$(DEFAULT): $(OBJECT_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)
    
$(PART1):$(OBJECT_FILES)
	rm -f $(PART1) $(PART2) $(DEFAULT)
	$(CC) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJECT_FILES)
    
$(PART2):$(OBJECT_FILES)
	rm -f $(PART1) $(PART2) $(DEFAULT)
	$(CC) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJECT_FILES)