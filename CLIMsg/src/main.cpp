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

#define MAX_SEND_SIZE 80

struct mymsgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

int queueID;
mymsgbuf buffer;

void usage(void)
{
	fprintf(stderr, "msgtool - A utility for tinkering with msg queues\n");
	fprintf(stderr, "\nUSAGE: msgtool (s)end <type> <messagetext>\n");
	fprintf(stderr, "	       (r)ecv <type>\n");
	fprintf(stderr, "	       (d)elete\n");
	fprintf(stderr, "	       (m)ode <octal mode>\n");
	exit(1);
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

int main(int argc, char *argv[])
{
	key_t key;

	buffer.mtype=1;
	buffer.mtext[0]=0;

	if(argc==1)
		usage();

	/* Create unique key via call to ftok() */
	key=ftok(argv[0],'k');

	/* Open the queue - create if necessary */
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



