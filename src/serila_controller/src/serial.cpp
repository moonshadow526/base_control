//
// Created by chke on 7/14/17.
//

#include "../include/serial.h"
Serial::Serial()
{

}

Serial::~Serial()
{

}

int Serial::open_port(int index)
{
    int fd;
    char *dev[] = {"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};
    if (index > 0 && index <=3){
        printf("%s\n",dev[index-1]);
        fd = open(dev[index-1],O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd){
            perror("Can't open serial port");
            return -1;
        }
        else
            printf("Open %s ....\n",dev[index -1]);
    }

    if (fcntl(fd, F_SETFL, 0)<0)
        printf("Fcntl failed!\n");
    else
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    if (isatty(STDIN_FILENO) == 0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");
    printf("fd-open = %d\n",fd);
    return fd;
}

int Serial::set_port(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;
    if (fd < 0)
        return -1;
    if  ( tcgetattr(fd, &oldtio)  !=  0)
    {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':                     //奇校验
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':                     //偶校验
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':                    //无校验
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int Serial::read_port(int fd, unsigned char *data, int datalength)
{
    if (fd < 0)
        return -1;
    int len = 0;
    memset(data,0,datalength);

    int max_fd = 0;
    fd_set read_set = {0};
    struct timeval tv = {0};

    FD_ZERO(&read_set);
    FD_SET(fd,&read_set);
    max_fd = fd+1;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (select(max_fd, &read_set, NULL, NULL, NULL) <0)
    {
        printf("Read data select error\n");
    }
    int nRet = FD_ISSET(fd, &read_set);
    if (nRet)
    {
        len = read(fd, data, datalength);
    }
    return len;
}

int Serial::write_port(int fd, const char *data, int datalength)
{
    if (fd < 0)
        return -1;
    int len = 0, total_len = 0;
    for (total_len = 0; total_len < datalength;){
        len = write(fd,&data[total_len], datalength-total_len);
        printf("WriteData fd = %d ,len =%d,data = %s\n",fd,len,data);
        if (len > 0)
        {
            total_len +=len;
        }
        else
        {
            len = -1;
            break;
        }
    }
    return len;
}

void Serial::close_port(int fd)
{

    close(fd);
}

