#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define X_MAX 1187
#define Y_MAX 1895 

char *portname = "/dev/ttyUSB0";
int fd;
int wlen;

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 10;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}

int initComm(){
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B9600);
    //set_mincount(fd, 0);                /* set to pure timed read */
    return 0;
}

int send(const char* str, int size){
    /* simple output */
    wlen = write(fd, str, size);
    if (wlen != size) {
        printf("Error from write: %d, %d\n", wlen, errno);
        return -1;
    }
    tcdrain(fd);    /* delay for output */
    return 0;
}

int waitTilReady(){
    /* simple noncanonical input */
    do {
        unsigned char buf[80];
        int rdlen;

        rdlen = read(fd, buf, sizeof(buf) - 1);
        if (rdlen > 0) {
#ifdef DISPLAY_STRING
            buf[rdlen] = 0;
            printf("Read %d: \"%s\"\n", rdlen, buf);
#endif      
            buf[rdlen] = 0;
            printf("|%s|\n", buf);
            if( strncmp(buf, "SystemReady", 11) == 0){
                usleep (500000);             // sleep enough to transmit the 7 plus
                return 0;
            }
        } 
#ifdef VERBOSE
        else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
            return -1;
        } else {  /* rdlen == 0 */
            printf("Timeout from read\n");
        }
#endif                   
        /* repeat read to get full message */
    } while (1);
}

int getMsgSize(char *msg){
    int size = 0;
    while(msg[size]!='\n')size++;
    return size+1;
}

void drawCircle(int i, int j, int size){
    int width  = X_MAX;
    int height = X_MAX;
    if(size == 1)height = Y_MAX;
    else if(size == 2)width = height = 900;

    int xSpace = width/3;
    int ySpace = height/3;

    int x = j*xSpace+(xSpace/2);
    int y = i*ySpace+(ySpace/2);
    int r = MIN(xSpace/2, ySpace/2);

    int  commandLen = 32;
    char command[commandLen];

    sprintf(command, "1:%d:%d:%d:\n", x, y, r);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:0:0:\n");
    send(command,getMsgSize(command));
    waitTilReady();
}

void drawX(int i, int j, int size){
    int width  = X_MAX;
    int height = X_MAX;
    if(size == 1)height = Y_MAX;
    else if(size == 2)width = height = 900;

    int xSpace = width/3;
    int ySpace = height/3;

    int  commandLen = 16;
    char command[commandLen];

    sprintf(command, "0:%d:%d:\n", j*xSpace, i*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "1:%d:%d:\n", j*xSpace+xSpace, i*ySpace+ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:%d:%d:\n", j*xSpace+xSpace, i*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "1:%d:%d:\n", j*xSpace, i*ySpace+ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:0:0:\n");
    send(command,getMsgSize(command));
    waitTilReady();

}

void drawTriangle(int i, int j, int size){
    int width  = X_MAX;
    int height = X_MAX;
    if(size == 1)height = Y_MAX;
    else if(size == 2)width = height = 900;

    int xSpace = width/3;
    int ySpace = height/3;

    int  commandLen = 16;
    char command[commandLen];

    sprintf(command, "0:%d:%d:\n", j*xSpace+(xSpace/2), i*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "1:%d:%d:\n", j*xSpace, i*ySpace+(ySpace/2));
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "1:%d:%d:\n", j*xSpace+xSpace, i*ySpace+(ySpace/2));
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "1:%d:%d:\n", j*xSpace+(xSpace/2), i*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:0:0:\n");
    send(command,getMsgSize(command));
    waitTilReady();
}


void drawWin(int i, int size){
    int width  = X_MAX;
    int height = X_MAX;
    if(size == 1)height = Y_MAX;
    else if(size == 2)width = height = 900;

    int xSpace = width/3;
    int ySpace = height/3;

    int  commandLen = 16;
    char command[commandLen];

    if(i < 3){ // ROW
        sprintf(command, "0:%d:%d:\n", 0, i*ySpace+(ySpace/2));
        send(command,getMsgSize(command));
        waitTilReady();

        sprintf(command, "1:%d:%d:\n", width, i*ySpace+(ySpace/2));
        send(command,getMsgSize(command));
        waitTilReady();
    }else if(i < 6){// COLUMN
        i-=3;
        sprintf(command, "0:%d:%d:\n",  i*xSpace+(xSpace/2), 0);
        send(command,getMsgSize(command));
        waitTilReady();

        sprintf(command, "1:%d:%d:\n", i*xSpace+(xSpace/2), height);
        send(command,getMsgSize(command));
        waitTilReady();
    }else if(i == 6){// DIAGONAL UPPER_LEFT-BOTTOM_RIGHT
        sprintf(command, "0:%d:%d:\n",  0, 0);
        send(command,getMsgSize(command));
        waitTilReady();

        sprintf(command, "1:%d:%d:\n", width, height);
        send(command,getMsgSize(command));
        waitTilReady();
    }else if(i == 7){// BOTTOM_LEFT-UPPER_RIGHT
        i-=3;
        sprintf(command, "0:%d:%d:\n",  0, height);
        send(command,getMsgSize(command));
        waitTilReady();

        sprintf(command, "1:%d:%d:\n", width, 0);
        send(command,getMsgSize(command));
        waitTilReady();
    }

    sprintf(command, "0:0:0:\n");
    send(command,getMsgSize(command));
    waitTilReady();

}


void drawGrid(int size){
    int width  = X_MAX;
    int height = X_MAX;
    if(size == 1)height = Y_MAX;
    else if(size == 2)width = height = 900;

    int xSpace = width/3;
    int ySpace = height/3;

    int  commandLen = 16;
    char command[commandLen];
    sprintf(command, "0:%d:0:\n", xSpace);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "1:%d:%d:\n", xSpace, height);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "0:%d:%d:\n", 2*xSpace, height);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "1:%d:0:\n", 2*xSpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:0:%d:\n", ySpace);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "1:%d:%d:\n", X_MAX, ySpace);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "0:%d:%d:\n", X_MAX, 2*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();
    sprintf(command, "1:0:%d:\n", 2*ySpace);
    send(command,getMsgSize(command));
    waitTilReady();

    sprintf(command, "0:0:0:\n");
    send(command,getMsgSize(command));
    waitTilReady();
}