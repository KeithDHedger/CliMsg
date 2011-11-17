/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: msgtool.c
 *****************************************************************************
 A command line tool for tinkering with SysV style Message Queues
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <getopt.h>

#define MAX_SEND_SIZE 80
#define VERSION "0.0.0"
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

int queueID;
mymsgbuf buffer;
int msgType=1;

void printhelp(void)
{
printf("Usage: climsg [OPTION]\n"
	"A CLI application\n"
	" -s, --send	Send message\n"
	" -r, --receive	Receive message\n"
	" -t, --type	Message type (defaults to 1)\n"
	" -d, --delete	Delete message queue\n"
	" -v, --version	output version information and exit\n"
	" -h, -?, --help	print this help\n\n"
	"Report bugs to kdhedger@yahoo.co.uk\n"
	);
}


void send_message(char *text)
{
	strcpy(buffer.mtext,text);

	if((msgsnd(queueID,&buffer,strlen(buffer.mtext)+1,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(1);
		}
}


void read_message()
{
	int retcode;

	retcode=msgrcv(queueID,&buffer,MAX_SEND_SIZE,1,IPC_NOWAIT);

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
	while (1)
		{
		int option_index = 0;
		c = getopt_long (argc, argv, "v?hds:r:t:",long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
			{
			case 's':
				printf("Send Arg=%s\n",optarg);
				break;
			case 'r':
				printf("Xceive Arg=%s\n",optarg);
				break;
			case 't':
				msgType=strtol(optarg,NULL,10);
				printf("Type Arg=%s msgType=%i\n",optarg,msgType);
				break;
		
			case 'd':
				printf("delete message queue\n");
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
	
	if (optind < argc)
		{
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
		}
	
	printf("%s\n","Hello World");
	return 0;
}

/*
int mainXX(int argc, char *argv[])
{
	key_t key;

	buffer.mtype=1;
	buffer.mtext[0]=0;

	if(argc==1)
		usage();

	// Create unique key via call to ftok()
	key=ftok(argv[0],'k');

	// Open the queue - create if necessary
	if((queueID=msgget(key,IPC_CREAT|0660))==-1)
		{
			perror("msgget");
			exit(1);
		}

	switch(tolower(argv[1][0]))
		{
			case 's':
				send_message(argv[2]);
				break;
			case 'r':
				read_message(); 
				break;
			case 'd':
				remove_queue(); 
				break;	

			default:
				usage();
		}

	return(0);
}

*/

