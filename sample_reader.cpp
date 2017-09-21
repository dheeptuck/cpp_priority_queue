/**
 * @brief      This is a sample reader class. Keeps reading the messages
 *             while keeping a track on the number of read messages. Once
 *             no message is read for more than 2 seconds. The thread exits.
 */ 

#include "sample_reader.h"

/**
 * @brief      Constructor
 *
 * @param      queue  The queue from which message needs to be read
 * @param      cnt    The variable that will keep track of the received
 *                    message count.
 */
Reader::Reader(PriQueue &queue, uint32_t *cnt)
:queue_(queue),
 msg_cnt(cnt),
 last_ts_(clock()) {
}
    
/**
 * @brief      Keeps reading the message and times out if no message
 *             read for more than 2 seconds.
 */
void Reader::run() {
    uint32_t _timer = 0;
    last_ts_ = clock();
    msg_t tmp;
    while(1) {
        if(queue_.GetEntry(&tmp)) {
            last_ts_ = clock();
            // Increment message only if the last byte in the
            // message is same as the priority.
            if(0xcd == tmp.data[MSG_LEN-1]) {
                (*msg_cnt)++;    
            }
            
        }

        // Timeout if data not recieved for 5 seconds
        if((double)(clock()-last_ts_) / ((double)CLOCKS_PER_SEC) > 2) {
            break;
        }
    }
    return; 
}