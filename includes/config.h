/**
 * This file holds the configuration seeting for the system.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_


#define MSG_LEN 4                               // Length of message in the 
                                                // priority msg queue

#define DFLT_MIN_THRESHOLD 5                    // The default threshold at
                                                // which MIN element event
                                                // needs to be raised

#define DFLT_MAX_THRESHOLD 15                   // The default threshold at
                                                // which MAX element event
                                                // needs to be raised

#define MAX_WRITERS 3                           // The maximum number of writer
                                                // threads possible

// TEST CONF

// ReaderWriterTest Conf(Refer Readme)
#define TEST_2_QUEUE_SIZE        10
#define MESSAGE_CNT_PER_THREAD   5000
#define HIGH_WATER_MARK			 (TEST_2_QUEUE_SIZE*9)/10    // Set min
                      			                             // threshold to
                      			                             // 10%
#define LOW_WATER_MARK          TEST_2_QUEUE_SIZE/10         // Set max 
                      			                             // threshold 
                      			                             // to 90%

#endif