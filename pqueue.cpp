#include "pqueue.h"


using namespace std;


/**
 * @brief      { Constructs the Q }
 *
 * @param[in]  max_size  The maximum size of the Q.
 */
PriQueue::PriQueue(uint32_t max_size):
queue_ptr_(0),
max_q_sz_(0),
current_q_sz_(0),
call_back_cnt_(0),
last_evt_(ALMOST_EMPTY),
q_locked_(false),
min_threshold_(DFLT_MIN_THRESHOLD),
max_threshold_(DFLT_MAX_THRESHOLD)
{
    max_q_sz_ = max_size;
    queue_ptr_ = new  msg_t[max_size];
    for(uint16_t i=0;i < MAX_WRITERS;i++) {
        cb_array_[i] = NULL;
    }
}


/**
 * @brief      Puts an entry onto the Q.
 *
 * @param[in]  msg   The message to be populated.
 *
 * @return     true if the message was successfully populated
 */
bool PriQueue::PutEntry(msg_t msg) {
    while(true == q_locked_);       // Wait till lock released
    q_locked_ = true;               // Lock Queue so that the below code is used
                                    // only by one thread
    if (max_q_sz_ == current_q_sz_) {
        q_locked_ = false;
        return false;
    }
    ++current_q_sz_;
    queue_ptr_[current_q_sz_-1] = msg;

    for(uint32_t i=current_q_sz_; i != 0; i--) {
        if(queue_ptr_[i-1].priority > ParentNode_(i-1)->priority) {
            // swap messages
            msg_t temp;       
            memcpy(&temp           , &queue_ptr_[i-1], sizeof(temp));
            memcpy(&queue_ptr_[i-1], ParentNode_(i-1), sizeof(temp));
            memcpy(ParentNode_(i-1), &temp           , sizeof(temp));
        }
    }

    // Iterate over the interface objects to call callbacks for Q almost full
    if(ALMOST_EMPTY == last_evt_) {
        if(current_q_sz_ >= max_threshold_) {
            for(uint16_t i=0; i<call_back_cnt_; i++) {
                cb_array_[i]->evt_cb(ALMOST_FULL);
            }
            last_evt_ = ALMOST_FULL;
        }
       
    }
    q_locked_ = false;             // Release lock so that other threads can
                                   // access
    return true;
}

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
bool PriQueue::GetEntry(msg_t *msg) {
    while(true == q_locked_);       // Wait till lock released
    q_locked_ = true;               // Lock Queue so that the below code is used
                                    // only by one thread
                                    
    if(0 == current_q_sz_) {        // Exit if wrong call
        q_locked_ = false;          // Release lock
        return false;
    }
    
    *msg = queue_ptr_[0];
    queue_ptr_[0] = queue_ptr_[current_q_sz_-1];
    current_q_sz_--;
    RestructHeap_(0);

    // Call callbacks for Q almost empty
    if(ALMOST_FULL == last_evt_) {
        if(current_q_sz_ <= min_threshold_) {
            for(uint16_t i=0; i<call_back_cnt_; i++) {
                cb_array_[i]->evt_cb(ALMOST_EMPTY);
            }
            last_evt_ = ALMOST_EMPTY;
        }
    }
    q_locked_ = false;             // Release lock so that other threads can
                                   // access
    return true;
}

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
bool PriQueue::RegQEventCb(WriterInterFace &interface) {

    // Raise exception if more CB's registered
    if(call_back_cnt_ == MAX_WRITERS) {
        return false;
    }
    cb_array_[call_back_cnt_] = &interface;
    call_back_cnt_++;
}

//parse the max value trees assuming subtrees are Ok
void PriQueue::RestructHeap_(uint32_t idx) {

    int max_element_idx = idx;

    //Check for the max value in parent,left and right
    if( (current_q_sz_ > (idx*2 + 1) ) && 
                    (queue_ptr_[idx].priority < LeftNode_(idx)->priority) ) {
        max_element_idx = (idx*2 + 1);
    }
    if( (current_q_sz_ > (idx*2 + 2) ) && 
             (queue_ptr_[max_element_idx].priority < RightNode_(idx)->priority) ) {
        max_element_idx = (idx*2 + 2);
    }
    if(max_element_idx != idx) {
        // Swap messages
        msg_t temp;
        memcpy(&temp,            &queue_ptr_[max_element_idx], sizeof(temp));
        memcpy(&queue_ptr_[max_element_idx], &queue_ptr_[idx], sizeof(temp));
        memcpy(&queue_ptr_[idx]            , &temp           , sizeof(temp));
        RestructHeap_(max_element_idx);
    }

}

/**
 * @brief      Sets the minimum Q element event threshold.
 *
 * @param[in]  min_threshold_  The minimum threshold at which
 *                              
 */
void PriQueue::SetMinEvtThreshold(uint32_t min) {
    min_threshold_ = min;
}


/**
 * @brief      Sets the maximum Q element event threshold.
 *
 * @param[in]  max_threshold_  The maximum threshold at which
 *                              
 */
void PriQueue::SetMaxEvtThreshold(uint32_t max) {
    max_threshold_ = max;
}


/**
 * @brief      Gets the current queue size.
 *
 * @return     The queue size.
 */
uint32_t PriQueue::GetQueueSize() {
    return current_q_sz_;
}

PriQueue::~PriQueue() {
    delete [] queue_ptr_;
}

msg_t* PriQueue::RightNode_(int32_t idx) {
    return &queue_ptr_[idx*2 + 2];
}

msg_t* PriQueue::LeftNode_(int32_t idx) {
    return &queue_ptr_[idx*2 + 1];
}

msg_t* PriQueue::ParentNode_(int32_t idx) {
    return &queue_ptr_[(idx-1)/2];
}