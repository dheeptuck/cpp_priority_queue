/**
 * The file defines a interface for the writers. All the
 * writers need to inherit from this class and should
 * mandatorily have a implementation for evt_cb() and
 * run() functions.
 */
#ifndef _WRITER_INTERFACE_H_
#define _WRITER_INTERFACE_H_

#include "common_ds.h"

class WriterInterFace {

public:
	/**
	 * @brief      Interface function for callback.
	 *
	 * @param[in]  evt   The event call.
	 */
    virtual void evt_cb(Q_EVT evt)=0;

    /**
     * @brief      The write activity needs to be present
     * 			   in this function.
     */
    virtual void run()=0;

};

#endif
