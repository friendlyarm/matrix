#include     <stdio.h>      
#include     <stdlib.h>    
#include     <unistd.h>    
#include     <sys/types.h> 
#include     <sys/stat.h> 
#include     <fcntl.h>    
#include     <termios.h> 
#include     <errno.h> 
#include     <pthread.h>
#include     <string.h>

#define FALSE 0
#define TRUE 1

#define PRINTF oprintf

#include <libfahw.h>

int speed_arr[] = {B115200, B576000, B38400, B19200, B9600, B4800, B2400, B1200, B300,
	B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300,
	38400,  19200,  9600, 4800, 2400, 1200,  300, };

void set_speed(int fd, int speed)
{
	int   i;
	int   status;
	struct termios   Opt;
	tcgetattr(fd, &Opt);
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
	{
		if  (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0)
				perror("tcsetattr fd1");
			return;
		}
		tcflush(fd,TCIOFLUSH);
	}
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
	if  ( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) 
	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);
	}
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); 
			options.c_iflag |= INPCK;            
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     /* Enable parity */
			options.c_cflag &= ~PARODD;  
			options.c_iflag |= INPCK;    
			break;
		case 'S':
		case 's':  /*as no parity*/
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
	}
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported stop bits\n");
			return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	options.c_lflag = 0;

	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}

int OpenDev(char *Dev)
{
	int	fd = open( Dev, O_RDWR | O_NDELAY | O_NOCTTY);         //| O_NOCTTY | O_NDELAY
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		return fd;

}

int SerialOpen(char* dev, int baud, char* opt)
{
	int fd;
	int bits;
	int stopbits;
	char parity;
	fd = OpenDev(dev);
	if (fd>0)
		set_speed(fd,baud);
	else
	{
		printf("Can't Open Serial Port:%s!\n",dev);
		exit(0);
	}
	parity = opt[0];
	bits = opt[1] - '0';
	stopbits = opt[2] - '0';
	
	switch(parity){
	case 'N':
	case 'O':
	case 'E':
	case 'n':
	case 'o':
	case 'e':
	case 'S':
	case 's':
		break;
	default:
		printf("Parity false\n");
		exit(1);
	}
	
	if((stopbits != 1) && (stopbits != 2)){
		printf("Stopbits false\n");
		exit(1);
	}

	if((bits != 7) && (bits != 8)){
		printf("Bits false\n");
		exit(1);
	}
	
	if (set_Parity(fd,8,1,'N')== FALSE)
	{
		printf("Set Parity Error\n");
		exit(1);
	}
	return fd;
}

#define RX_BUF_SIZE 512
char rx_buf[RX_BUF_SIZE];
char now_time[8];
char time_fre = 0;
void *com_rx (void *arg)
{
	int serialfd = (int)arg;
	int nread = 0;
	int nback = 0;
	int timer = 0;
	int status = 0;
	while(1){
		nread = read(serialfd, rx_buf + nback, RX_BUF_SIZE - nback);
		if(nread > 0){
			nback += nread;
		}

		if(nread > 0){
			timer = 0;
			status = 1;
		}
		else if(status == 1){ 
			timer++;
		}
		
		if(status && (timer > 10)){
			rx_buf[nback] = '\0';
			char *p = strstr(rx_buf,"$GNRMC,");
			if(p){
				p += 7;
				unsigned char h = (p[0] - 48) * 10 + 40;
				now_time[0] = h/10 + 48;
				now_time[1] = h%10 + 48;
				now_time[2] = '-';
				now_time[3] = p[2];
				now_time[4] = p[3];
				now_time[5] = '-';
				now_time[6] = p[4];
				now_time[7] = p[5];
				now_time[8] = '\0';
				time_fre = 1;
			}
			status = 0;
			timer = 0;
			memset(rx_buf,'\0',nback);
			nback = 0;
		}
		usleep(1000);
	}
}

struct oled_cursor{
	int x;
	int y;
};

extern struct oled_cursor __oled_cursor;

int main (void)
{
	pthread_t rx_thread;
	boardInit();
	int SerialFD = SerialOpen("/dev/ttyS1",115200,"N81");
	OLEDInit(7,11);
	OLEDCleanScreen();
	if(SerialFD > 0){
		PRINTF("Serial Opened\n");
	}else{
		PRINTF("Serial Open failed\n");

	}
	
        pthread_create(&rx_thread,NULL,com_rx,(void*)SerialFD);
	
	while(1){
		if(time_fre){
			time_fre = 0;
			__oled_cursor.x = 0;
			__oled_cursor.y = 1;
			PRINTF("Time:%s",now_time);
		}
	}

	return 0;
}
