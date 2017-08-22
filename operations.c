#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "operations.h"
static int seconds = 0;
static int time = 0;
static char pos = '|';
static char neg = ' ';
static int done = 0;
/* returns 0 on success, 1 on failure. */
int verify(char * string)
{
    int x = 0;
    int len = strlen(string);

    while(x < len) {

           if((x != 0 || *(string+x) != '-') && !isdigit(*(string+x)))
           	return 1;
           ++x;
    }
    return 0;
}
int setupTimer(int argc, char ** argv)
{
	int temp;
	if(argc > 2 || argc < 2)
	{
		fprintf(stderr, "usage: //home/getaylor/mytimer <seconds>\n");
		exit(-1);
	}
	if(verify(argv[1]) == 1)
	{
		fprintf(stderr, "\"%s\": malformed time.\nusage: //home/getaylor/mytimer <seconds>\n", argv[1]);
		exit(-1);
	}
	temp = atoi(argv[1]);
	if(temp < 0)
	{
		fprintf(stderr, "Invalid time(%d).  Must be >= 0.\nusage: //home/getaylor/mytimer <seconds>\n", temp);
		exit(-1);
	}
	seconds = temp;
	return 0;

}

void printTime()
{
	static char * spin = "|/-\\";
	static int bar = 1;
	
	printf("                                                 \r%c %c%d:%02d:%02d\r", pos, neg, abs(seconds / 3600), abs((seconds % 3600) / 60), abs(seconds % 60));
	fflush(stdout);
	if(bar == 4)
		bar = 0;
	pos = spin[bar];
	bar++;
}/*\33[2K*/
void empty_handler(int signum)
{

}
void alrm_handler(int signum)
{
	if(done == 0 && seconds == 0 && time == 0)
	{
		printf("                                                 \r%cBeeeep! Time's up!\n", 7);
		fflush(stdout);
		done = 1;
		time++;
		return;
	}
	
	if(time == TICKS_PER_SECOND)
	{
		seconds--;
		time = 0;
		if(seconds < 0)
		{
			neg = '-';
		}
		else
		{
			neg = ' ';
		}
		if(seconds == 0 && time == 0)
		{
			printf("                                                 \r%cBeeeep! Time's up!\n", 7);
			fflush(stdout);
		}
	}
	

	time++;
	
	printTime();
}

struct termios setupTerminal()
{
	struct termios old_tio, new_tio;
	tcgetattr(0, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag &= ~( ICANON | ECHO | ISIG);
	tcsetattr(0,TCSANOW,&new_tio);
	return old_tio;
}
int characterHandler(char c, struct sigaction sa, struct sigaction empty)
{
	if(c == 'q')
	{
		sigaction(SIGALRM, &empty, NULL);
		return 1;
	}
	else if(c == 'c')
	{
		seconds = 0;
		time = 0;
	}
	else if(c == 'r')
	{
		static int toggle = 0;
		if(toggle == 0)
		{
			sigaction(SIGALRM, &empty, NULL);
			toggle = 1;
		}
		else
		{
			sigaction(SIGALRM, &sa, NULL);
			toggle = 0;
		}
	}
	else if(c == 'h')
	{
		seconds += 3600;
	}
	else if(c == 'H')
	{
		seconds -= 3600;
	}
	else if(c == 'm')
	{
		seconds += 60;
	}
	else if(c == 'M')
	{
		seconds -= 60;
	}
	else if(c == 's')
	{
		seconds++;
	}
	else if(c == 'S')
	{
		seconds--;
	}
	else
	{
		return 0;
	}
	if(seconds < 0)
	{
		neg = '-';
	}
	else
	{
		neg = ' ';
	}
	printTime();
	return 0;
}
//sigdafl, sigign
