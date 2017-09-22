//
// Created by chke on 7/14/17.
//

#include "../include/serial.h"
#include "../include/read_encoder.h"

//int hotplug_sock;
//#define UEVENT_BUFFER_SIZE 2048

//static int init_hotplug_sock()
//{
//    const int buffersize = 1024;
//    int ret, on=1;
//
//    struct sockaddr_nl snl;
//    bzero(&snl, sizeof(struct sockaddr_nl));
//    snl.nl_family = AF_NETLINK;
//    snl.nl_pid = getpid();
//    snl.nl_groups = 1;
//
//    int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
//    if (s == -1)
//    {
//        perror("socket");
//        return -1;
//    }
//    setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
//
//    ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));
//    if (ret < 0)
//    {
//        perror("bind");
//        close(s);
//        return -1;
//    }
//
//    return s;
//}

int main()
{

    int fd = 0;

    MotoControl motoControl;

    fd = motoControl.open_encoder_port(1);
    if (0 == fd){
        motoControl.read_encodet_task();
    }

//    Control.close_encoder_port();
    else{
        while (-1 == fd)
        {
            fd = motoControl.open_encoder_port(1);
            sleep(1);
        }
        motoControl.read_encodet_task();
    }
    while (1);
}