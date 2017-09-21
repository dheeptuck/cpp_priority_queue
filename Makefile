#------------------------------------------------------------------------------

SOURCE=pqueue.cpp test_runner.cpp sample_writer.cpp sample_reader.cpp
PROGNAME=binary/p_queue
INCLUDES=$(shell pwd)/includes

CC=g++
#------------------------------------------------------------------------------


all: $(PROGNAME)

run: $(PROGNAME)
	 $(shell echo ./$(PROGNAME))
$(PROGNAME): $(SOURCE)
	$(CC) -I$(INCLUDES) $(SOURCE) -o$(PROGNAME) -std=c++11 -pthread

clean:
	rm -f $(PROGNAME)
