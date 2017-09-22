//
// Created by chke on 7/14/17.
//

#include "../include/read_encoder.h"

MotoControl::MotoControl()
{
    encoder_fd = 3;
}

MotoControl::~MotoControl()
{

}

void * read_encoder_fun(void * para)
{
    MotoControl motoControl;
    MotoControl *pMotoControl = (MotoControl *)para;
    while (1) {
            pMotoControl ->read_encoder_data();
    }
}

int MotoControl::open_encoder_port(int index)
{
    encoder_fd = open_port(index);
    if (encoder_fd < 0){
        printf("Open channel encoder error");
        return -1;
    }
    else{
        printf("Open channel encoder success!\n");

        if(set_port(encoder_fd,115200,8,'N',1)<0)
        {
            perror("set_opt error");
            return -1;
        }
    }
    tcflush(encoder_fd, TCIFLUSH);
    return 0;
}

int MotoControl::close_encoder_port()
{

    close_port(encoder_fd);
    return 0;
}

int MotoControl::read_encoder_data()
{
    printf("encoder_fd is %d\n",encoder_fd);
    if (read_port(encoder_fd,encoder_data,30) > 0)
    {
        printf("read data is %s\n",encoder_data);

        return 0;
    }
    else{
        if (0 <= (write_encoder_data())){
            printf("read data error");
            return -1;
        }
        else{
            printf("prot is miss\n");
            close_encoder_port();
            while ( -1 == open_encoder_port(1))
            sleep(1);
        }
    }

}

int MotoControl::read_encodet_task()
{
    int pid,ppid;
    if (pthread_create(&read_encoder_threadid, NULL, read_encoder_fun, this) != 0)
    {
        printf("creat read_encodet_task failed");
        close_port(encoder_fd);
        return -1;
    }
    printf("creat read_encodet_task success\n");
    pid = getpid();
    ppid = getppid();
    printf("%d,%d\n",pid,ppid);
    return 0;
}

int MotoControl::write_encoder_data()
{
    if (write_port(encoder_fd, "ifconnect",11) < 0){
        return -1;
    }
    return 0;
}