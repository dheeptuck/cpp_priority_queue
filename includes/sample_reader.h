/**
 * @brief      This is a sample reader class. Keeps reading the messages
 *             while keeping a track on the number of read messages. Once
 *             no message is read for more than 2 seconds. The thread exits.
 */ 
#ifndef _SAMPLE_READER_H_
#define _SAMPLE_READER_H_

// std includes
#include <stdint.h>

// Custom includes
#include "pqueue.h"

class Reader {

private:
    PriQueue &queue_;
    Q_EVT q_state_;
    uint32_t *msg_cnt;
    clock_t last_ts_;
public:
    /**
     * @brief      Constructor
     *
     * @param      queue  The queue from which message needs to be read
     * @param      cnt    The variable that will keep track of the received
     *                    message count.
     */
    Reader(PriQueue &queue, uint32_t *cnt);
    
    /**
     * @brief      Keeps reading the message and times out if no message
     *             read for more than 2 seconds.
     */
    void run();


};

#endif