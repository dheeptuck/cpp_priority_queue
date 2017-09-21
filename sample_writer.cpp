/**
 * @brief      This is a sample writer class an it keeps posting
 *             messages to the queue till the Q is ALMOST_FULL 
 *             event and restores publishing after ALMOST_EMPTY
 *             event. This action keeps repeating till the max_msg_cnt
 *             that is passed as the constructor arguement.
 */
#include "sample_writer.h"



/**
 * @brief      { function_description }
 *
 * @param      queue        The queue to which the messages need to
 *                          be published
 * @param[in]  max_msg_cnt  The maximum messages that needs to be
 *                          published.
 */
Writer::Writer(PriQueue &queue, uint32_t max_msg_cnt)
    :queue_(queue),
     pri_(0),
     q_state_(ALMOST_EMPTY),
     msg_cnt_(max_msg_cnt) {
        queue_ = queue;
        queue_.RegQEventCb(*this);
    }

/**
 * @brief      Keeps publishing till the max_msg_cnt.
 *             Pauses if the ALMOST_FULL event is set.
 */
void Writer::run() {
    
    while(1) {

        msg_t tmp;
        tmp.priority = pri_;
        tmp.data[0] = 200;
        tmp.data[MSG_LEN - 1] = 0xcd;
        if(ALMOST_EMPTY == q_state_) {
            if(pri_ == msg_cnt_) {
                break;
            }
            if(!queue_.PutEntry(tmp)) {
                // ToDo: Raise Exception
            }

            pri_++;
        }
    }
    return;

}


/**
 * @brief      Callback that will be called by the priority queue
 *
 * @param[in]  evt   The event that will be passed by the CB handler
 */
void Writer::evt_cb(Q_EVT evt) {
    q_state_ = evt;
}