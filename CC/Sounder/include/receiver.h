/*
 Copyright (c) 2018-2019, Rice University 
 RENEW OPEN SOURCE LICENSE: http://renew-wireless.org/license
 Author(s): Peiyao Zhao: pdszpy19930218@163.com 
            Rahman Doost-Mohamamdy: doost@rice.edu
 
----------------------------------------------------------
 Handles received samples from massive-mimo base station 
----------------------------------------------------------
*/

#ifndef DATARECEIVER_HEADER
#define DATARECEIVER_HEADER
 
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <pthread.h>
#include <cassert>
#include <chrono>
#include "concurrentqueue.h"
#include "sdr-lib.h"

typedef unsigned short ushort;

struct complex_float {
    float real;
    float imag;
};

struct Event_data
{
    int event_type;
    int data;
};

struct SocketBuffer
{
    std::vector<char> buffer;
    std::vector<int> buffer_status;
};

class Receiver
{
public:
    
    // use for create pthread 
    struct ReceiverContext
    {
        Receiver *ptr;
        int tid;
    };

public:
    Receiver(int N_THREAD, Config *cfg);
    Receiver(int N_THREAD, Config *cfg, moodycamel::ConcurrentQueue<Event_data> * in_queue);
    ~Receiver();
    
    std::vector<pthread_t> startRecv(void** in_buffer, int** in_buffer_status, int in_buffer_frame_num, int in_buffer_length, int in_core_id=0);
    static void* loopRecv(void *context);
 
private:
    Config *config_;
    struct sockaddr_in servaddr_;    /* server address */
    int* socket_;

    RadioConfig *radioconfig_;

    void** buffer_;
    int** buffer_status_;
    int buffer_length_;
    int buffer_frame_num_;

    int thread_num_;
    // pointer of message_queue_
    moodycamel::ConcurrentQueue<Event_data> *message_queue_;
    int core_id_;

    ReceiverContext* context;
};

#endif