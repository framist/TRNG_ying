/**
 * @file slave.c
 * @author framist (framist@163.com)
 * @brief 下位机串口控制，需要root权限 
 * @version 0.1
 * @date 2021-10-17
 * 
 */

#include "slave.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>      // 文件控制
#include <termios.h>    // linux 串口驱动头文件


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) {
	struct termios newtio,oldtio;
	if  ( tcgetattr( fd,&oldtio)  !=  0) { 
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
 
	switch( nBits ) {
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}
 
	switch( nEvent ) {
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}
 
	switch( nSpeed ) {
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
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
	newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
//	printf("set done!\n\r");
	return 0;
}

static int Usart_fd;

u_int32_t slave_ntropy_solve(char * buffer){
    u_int32_t ntropy;
    sscanf(buffer,"%08x\n",&ntropy);
    return ntropy;
}

int slave_connect(const char *usartName) {
	if((Usart_fd = open(usartName,O_RDWR|O_NOCTTY|O_NDELAY)) < 0) {
		printf("open %s failed \n",usartName);
		return -1;
	} else { 
		printf("open %s success \n ",usartName);
		set_opt(Usart_fd,115200,8,'N',1); 
	}
    return 0;
}



int slave_read(char * buffer) {
    int nByte = 0;
    memset(buffer,0,sizeof(buffer));	//此句不知道为什么不用不行
    while(1) {
        while((nByte = read(Usart_fd,buffer,SLAVE_MSG_LEN_MAX)) > 0) {
            buffer[nByte + 1] = '\0';
            return 0;        
        }
    } 
}

int slave_write(enum HOST_MSG msg) {
	char buffer[] = "0\n";
    buffer[0] += msg;
    
    if( write(Usart_fd, buffer, strlen(buffer)) == 2) {
        // 成功写入2字符
    } else {
        printf("write failed \n");
        return -1;
    }
    return 0;
}

int slave_close(void) {
    close(Usart_fd);
}


#if DBG_SLAVE

int main(int argc , char* argv[])
{
    int i = 0;
	char *usart = "/dev/ttyUSB0";
    char buffer[SLAVE_MSG_LEN_MAX];
	memset(buffer,0,sizeof(buffer));	

    if(slave_connect(usart)==-1) return -1;
    slave_write(HOST_MSG_ACK);
    sleep(10);
    for(i = 0; i<10; i++) {
        slave_write(HOST_MSG_ACK);
        slave_read(buffer);
        printf("read: [%s]daer\n",buffer);
        printf("e:%u\n",slave_ntropy_solve(buffer));
    }

    slave_close();
	
	return 0;
}
 
#endif
