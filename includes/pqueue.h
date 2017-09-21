/**
 * This class implemets a priority Queue. The algorithm for the same
 * is implemented using max binary heap to have lower time complexity.
 * Please Note not all the functions are re-entrant but concurrecy is
 * achieved using a lock mechanism. This renders few of the API's
 * blocking.
 */
#ifndef _P_QUEUE_H_
#define _P_QUEUE_H_

// std includes
#include <climits>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

//Custom Includes
#include "writer_interface.h"
#include "common_ds.h"
#include "config.h"


/**
 * @brief      { Max Heap }
 * 
 * Key logic
 * parent = (i-1)/1
 * left node = 2*i + 1
 * right node = 2*i + 2
 */


class PriQueue {
public:
    /**
     * @brief      { Constructs the Q }
     *
     * @param[in]  max_size  The maximum size of the Q.
     */
    explicit PriQueue(uint32_t max_size);

    /**
     * @brief      Puts an entry onto the Q.
     *
     * @param[in]  msg   The message to be populated.
     *
     * @return     true if the message was successfully populated
     */
    bool     PutEntry(msg_t msg);

    /**
     * @brief      Gets the highest prioriity entry from the Q. Please
     *             note once the entry is retrieved the entry will be
     *             automatically removed from the Q. This API calls a
     *             lock when called and any succesive calls to the API
     *             might be blocked till the lock is released by the
     *             first thread.
     *
     * @param      Pointer to the msg object to which the message needs
     *             to be populated.
     * @return     True if entry was retrieved
     */
    bool GetEntry(msg_t *msg);

    /**
     * @brief      Registers the writer classes. Note all the writers
     *             need too inherit from WriterInterFace class and need
     *             to have a imlementation for void evt_cb(Q_EVT). See
     *             WriterInterFace class for better understanding.
     *
     * @param      interface  The
     *
     * @return     true if the callback got registered.
     */
    bool     RegQEventCb(WriterInterFace &interface);

    /**
     * @brief      Gets the current queue size.
     *
     * @return     The queue size.
     */
    uint32_t GetQueueSize();


    /**
     * @brief      Sets the minimum Q element event threshold.
     *
     * @param[in]  min_threshold_  The minimum threshold at which
     *                              
     */
    void SetMinEvtThreshold(uint32_t min_threshold_);


    /**
     * @brief      Sets the maximum Q element event threshold.
     *
     * @param[in]  max_threshold_  The maximum threshold at which
     *                              
     */
    void SetMaxEvtThreshold(uint32_t max_threshold_);
    
    /**
     * @brief      Destroys the object.Internally dealloacates memory.
     */
             ~PriQueue();
private:
    /**
     * Holds a pointer to the array using a heap data structure.
     */
    msg_t    *queue_ptr_;

    /**
     * Holds the maximum size of the queue.
     */
    uint32_t max_q_sz_;

    /**
     * Holds the current size of the queue.
     */
    uint32_t current_q_sz_;

    /**
     * Array holding pointers to the registered writers.
     */
    WriterInterFace *cb_array_[MAX_WRITERS];

    /**
     * Represents the no of callbacks registered.
     */
    uint16_t call_back_cnt_;

    /**
     * The threshold at which ALMOST_EMPTY eventy needs to be raised.
     */
    uint32_t min_threshold_;

    /**
     * The threshold at which ALMOST_FULL event needs to be raised.
     */
    uint32_t max_threshold_;

    /**
     * Holds the lock status
     */
    bool q_locked_;
    
    /**
     * { last_evt_ }
     * Maintains the last event state so that ALMOST_FULL
     * and ALMOST_EMPTY are only called alternatively.
     */
    Q_EVT last_evt_;

    /** Below ar few helper functions for the heap **/
    msg_t*   RightNode_(int32_t idx);
    msg_t*   LeftNode_(int32_t idx);
    msg_t*   ParentNode_(int32_t idx);
    void     RestructHeap_(uint32_t idx);

};


#endif