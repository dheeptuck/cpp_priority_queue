/**
 * @brief      This is a sample writer class an it keeps posting
 *             messages to the queue till the Q is ALMOST_FULL 
 *             event and restores publishing after ALMOST_EMPTY
 *             event. This action keeps repeating till the max_msg_cnt
 *             that is passed as the constructor arguement.
 */

#ifndef _SAMPLE_WRITER_H_
#define _SAMPLE_WRITER_H_

// std includes
#include <stdint.h>

//Custom includes
#include "writer_interface.h"
#include "pqueue.h"




class Writer:public WriterInterFace {
private:
    PriQueue &queue_;
    Q_EVT q_state_;
    uint16_t pri_;
    uint32_t msg_cnt_;
public:
    /**
     * @brief      { function_description }
     *
     * @param      queue        The queue to which the messages need to
     *                          be published
     * @param[in]  max_msg_cnt  The maximum messages that needs to be
     *                          published.
     */
    Writer(PriQueue &queue, uint32_t max_msg_cnt);

    void run();

    /**
     * @brief      Callback that will be called by the priority queue
     *
     * @param[in]  evt   The event that will be passed by the CB handler
     */
    void evt_cb(Q_EVT evt);
};

#endif