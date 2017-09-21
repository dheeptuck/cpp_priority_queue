// std includes
#include <thread>
#include <time.h>


// Custom includes
#include "pqueue.h"
#include "sample_writer.h"
#include "sample_reader.h"



using namespace std;


/**
 * @brief      Populates data in random order onto the queue
 *             and while retrieving checks weather the retrieval
 *             is based on the highest priority message first.
 */
void QFuncTest();


/**
 * @brief      Creates two readers and two writers to the queue
 *             Each writer populates 10,000 messages messgaes to
 *             the priority Queue. The sum total of the recieved
 *             messages should be equal to the sent messages else
 *             the test is a fail.
 */
void ReaderWriterTest();



int main() {
    
    // Run tests
    QFuncTest();
    ReaderWriterTest();
}



/**
 * @brief      Populates data in random order onto the queue
 *             and while retrieving checks weather the retrieval
 *             is based on the highest priority message first.
 */
void QFuncTest() {
    cout << "Running Test 1...." << endl;
    cout << "Running QFuncTest...." << endl;
    PriQueue pq(6);

    msg_t tmp;
    tmp.priority = -10;
    tmp.data[0] = 200;
    pq.PutEntry(tmp);

    tmp.priority = 15;
    tmp.data[0] = 201;
    pq.PutEntry(tmp);
    
    tmp.priority = 17;
    tmp.data[0] = 202;
    pq.PutEntry(tmp);

    tmp.priority = -19;
    tmp.data[0] = 205;
    pq.PutEntry(tmp);

    tmp.priority = 8;
    tmp.data[0] = 205;
    pq.PutEntry(tmp);

    tmp.priority = 9;
    tmp.data[0] = 205;
    pq.PutEntry(tmp);

    msg_t max_priority;
    pq.GetEntry(&max_priority);

    for(uint32_t i= pq.GetQueueSize();i>1;i--) {
        pq.GetEntry(&tmp);
        if(max_priority.priority < tmp.priority) {
            cout << "Test 1 fail" << endl;
            return;
        }
    }
    cout << "Test 1 pass" << endl;
    cout << "********************" << endl;
    return;

}


/**
 * @brief      Creates two readers and two writers to the queue
 *             Each writer populates 10,000 messages messgaes to
 *             the priority Queue. The sum total of the recieved
 *             messages should be equal to the sent messages else
 *             the test is a fail.
 */
void ReaderWriterTest() {
    cout << "Running Test 2...." << endl;
    cout << "Running ReaderWriterTest...." << endl;
    PriQueue pq(TEST_2_QUEUE_SIZE);

    // Set min threshold to 10%
    pq.SetMinEvtThreshold(LOW_WATER_MARK);
    if( 0 == LOW_WATER_MARK) {
        pq.SetMinEvtThreshold(1);
    }

    
    pq.SetMaxEvtThreshold(HIGH_WATER_MARK);
    if(TEST_2_QUEUE_SIZE < HIGH_WATER_MARK) {
        pq.SetMaxEvtThreshold(TEST_2_QUEUE_SIZE - 1);
    }

    // Stores the read messages per thread
    uint32_t rcv_cnt_1 = 0;
    uint32_t rcv_cnt_2 = 0;

    uint32_t send_cnt_1 = MESSAGE_CNT_PER_THREAD;
    uint32_t send_cnt_2 = MESSAGE_CNT_PER_THREAD;    

    // Create two writer threads to send 10K messages per thread
    Writer writer_1(pq, send_cnt_1);
    Writer writer_2(pq, send_cnt_2);

    // Create two reader threads
    Reader reader_1(pq, &rcv_cnt_1);
    Reader reader_2(pq, &rcv_cnt_2);

    // Create thread handlers
    thread t1(&Writer::run,ref(writer_1));
    thread t2(&Writer::run,ref(writer_2));
    thread t3(&Reader::run,ref(reader_1));
    thread t4(&Reader::run,ref(reader_2));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    if ( (rcv_cnt_1 + rcv_cnt_2) == (send_cnt_1 + send_cnt_2) ) {
        cout << "Test 2 pass" << endl;
        cout << "********************" << endl;
        return;
    }

    cout << "Test 2 fail" << endl;
    cout << "********************" << endl;

}
