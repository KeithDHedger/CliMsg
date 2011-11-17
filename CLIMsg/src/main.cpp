/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: msgtool.c
 *****************************************************************************
 A command line tool for tinkering with SysV style Message Queues
 *****************************************************************************/

/*****************************************************************************
 Tweaked by K.D.Hedger 2011
 kdhedger@yahoo.co.uk
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <getopt.h>

#define MAX_SEND_SIZE 80
#define VERSION "0.0.2"
#define UNKNOWNARG -100

struct option long_options[] =
	{
		{"send",1,0,'s'},
		{"receive",1,0,'r'},
		{"type",1,0,'t'},
		{"delete",0,0,'d'},
		{"version",0,0,'v'},
		{"help",0,0,'?'},
		{0, 0, 0, 0}
	};

struct mymsgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

int		queueID;
mymsgbuf	buffer;
int		msgType=1;
bool		action=false;

void printhelp(void)
{
	printf("Usage: climsg [OPTION]\n"
		"A CLI application\n"
		" -s, --send	Send message [STRING]\n"
		" -r, --receive	Receive message (defaults to receiving type 1)\n"
		" -t, --type	Message type (defaults to 1)\n"
		" -d, --delete	Delete message queue\n"
		" -v, --version	output version information and exit\n"
		" -h, -?, --help	print this help\n\n"
		"Report bugs to kdhedger@yahoo.co.uk\n"
		);
}


void send_message()
{
	if((msgsnd(queueID,&buffer,strlen(buffer.mtext)+1,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(1);
		}
}


void read_message()
{
	int retcode;

	retcode=msgrcv(queueID,&buffer,MAX_SEND_SIZE,msgType,IPC_NOWAIT);

	if(retcode>1)
		printf("%s\n",buffer.mtext);
}

void remove_queue()
{
	msgctl(queueID,IPC_RMID,0);
}

int main(int argc, char **argv)
{
	int c;
	key_t key;

	buffer.mtype=msgType;
	buffer.mtext[0]=0;

	while (1)
		{
		int option_index = 0;
		c = getopt_long (argc, argv, "v?hdrs:t:",long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
			{
			case 's':
				strcpy(buffer.mtext,optarg);
				action=true;
				break;
			case 'r':
				action=false;
				break;
			case 't':
				msgType=atoi(optarg);
				break;
		
			case 'd':
				remove_queue();
				return 0;
				break;

			case 'v':
				printf("climsg %s\n",VERSION);
				return 0;
				break;

			case '?':
			case 'h':
				printhelp();
				return 0;
				break;

			default:
				fprintf(stderr,"?? Unknown argument ??\n");
				return UNKNOWNARG;
			break;
			}
		}

	key=ftok(argv[0],'k');

	if((queueID=msgget(key,IPC_CREAT|0660))==-1)
		{
			fprintf(stderr,"Can't create message queue\n");
			exit(1);
		}

	buffer.mtype=msgType;
	if(action==true)
		send_message();
	else
		read_message();
	
}

