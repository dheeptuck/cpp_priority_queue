The project implements a priority queue and sample read and write routines.


QUICKSTART

1. Requirements - g++ (tested in 5.4.0)
                  make (tested in GNU Make 4.1)
                  Ubuntu-OS (Tested in 14.04)
2. Unzip the directory to the relevant location
3. Move to the directory.
4. excute the following commands in order
   make clean
   make all
   make run
5. The last in the above commands should run the tests
   if both tests pass we have successfully compiled
   and run the test cases in the project.
6. To change the test parameters edit // TEST CONF section in 
   includes/config.h to change the test parameters


TESTS
1. QFuncTest
First message is populated in inverse priority order to the queue and then
the data is read from the queue and checked weather the message is recieved
in the highest priority first order. Currently a 6 element Q is used. This
can be easily modified in test_runner.cpp.

2. ReaderWriterTest
Two threads continuously keep writing to the queue and two threads continuously
keep reading from the queue. The reader keeps a count of the valid messages
recieved. validity of the message is decided by checking weather the last byte
in the messsage is 0xcd; The test is considered a pass if the sum total of the
messages recieved by the two readers are same as the sum total sent by the two
writers. The high and low threshold are set to 90% and 10% respectively. 
See the // TEST CONF section in config.h to change the test parameters.
Following features are tested:
* Concurrency of the Priority Q API's
* Variable message length
* Functioning of the queue



DESIGN
The entire project can be broadly divided into three parts. The queue,
sample_reader and sample writer. Each of them are implemented as classes
in the relevant files.

1. sample_reader
This is a sample reader class. Keeps reading the messages
while keeping a track on the number of read messages. Once
no message is read for more than 2 seconds. The thread exits.
More on API's can be understood from the inline comments.


2. sample_writer
This is a sample writer class an it keeps posting
messages to the queue till the Q is ALMOST_FULL 
event and restores publishing after ALMOST_EMPTY
event. This action keeps repeating till the max_msg_cnt
that is passed as the constructor arguement.More on API's
can be understood from the inline comments.


3. pqueue
This class implemets a priority Queue. The algorithm for the same
is implemented using max binary heap to have lower time complexity.
Please Note not all the functions are re-entrant but concurrecy is
achieved using a lock mechanism. This renders few of the API's
blocking. More on API's can be understood from the inline comments.


OTHER SUPPORT FILES
1. include/config.h
   Includes configurations for the system.(MSG_LEN, DFLT_MIN_THRESHOLD etc)
2. writer_interface.h
   This is an interface class from which all writer classes need to inherit.
   This helps in providing a neat approach for event callback handling.
3. test_runner.cpp
   This file encompases the test and is the entry point to the system.




NOTES
1. Priority can be an integer(INT_MIN to INT_MAX)
   Assuming larger proirity value higher the priority
2. The queue is implemented using a binary heap.
3. The Queue is concurrent but blocking. (API's )
4. The message recieve is implemted in the sample_reader
   as a polling based mechnism.
5. All writers need to inherit from writer Interfaace