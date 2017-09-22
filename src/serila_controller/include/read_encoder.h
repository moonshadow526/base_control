//
// Created by chke on 7/14/17.
//

#ifndef PROJECT_READ_ENCODER_H
#define PROJECT_READ_ENCODER_H

#include "serial.h"
#include <sys/socket.h>

//extern int hotplug_sock;
//#define UEVENT_BUFFER_SIZE 2048

class MotoControl : public  Serial
{
public:
    MotoControl();
    ~MotoControl();
    int open_encoder_port(int index);
    int close_encoder_port();
    int read_encoder_data();
    int read_encodet_task();
    int write_encoder_data();

private:
    int encoder_fd;
    pthread_t read_encoder_threadid;
    unsigned char encoder_data[50];

};

#endif //PROJECT_READ_ENCODER_H
