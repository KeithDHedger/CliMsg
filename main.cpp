/************************************************************************
*									*
* SysV message queue send/receive					*
*									*
* K.D.Hedger 2011							*
*									*
* kdhedger@yahoo.co.uk							*
*									*
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <getopt.h>

#define APPNAME "climsg"
#define VERSION "0.0.7"

#define MAX_MSG_SIZE 1024

#define ALLOK 0
#define UNKNOWNARG 1
#define NOMAKEQUEUE 2
#define NOSENDMSG 3
#define WAIT_MSG 0

struct option long_options[] =
	{
		{"send",1,0,'s'},
		{"receive",1,0,'r'},
		{"type",1,0,'t'},
		{"delete",0,0,'d'},
		{"key",1,0,'k'},
		{"wait",0,0,'w'},
		{"version",0,0,'v'},
		{"help",0,0,'?'},
		{0, 0, 0, 0}
	};

struct msgBuffer
	{
		long mType;
		char mText[MAX_MSG_SIZE];
	};

int		queueID;
msgBuffer	buffer;
int		msgType=1;
bool		action=false;
int		receiveType=IPC_NOWAIT;

void printHelp()
{
	printf("Usage: %s [OPTION] [TEXT]\n"
		"A CLI application\n"
		" -s, --send	Send message [TEXT]\n"
		" -r, --receive	Receive message (defaults to receiving type 1)\n"
		" -t, --type	Message type (defaults to 1)\n"
		" -d, --delete	Delete message queue\n"
		" -k, --key	Use key [INTEGER] instead of generated one\n"
		" -w, --wait	Wait for message to arrive (blocking)\n"
		" -v, --version	output version information and exit\n"
		" -h, -?, --help	print this help\n\n"
		"Report bugs to kdhedger@yahoo.co.uk\n"
		,APPNAME);
}

void sendMsg()
{
	if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(NOSENDMSG);
		}
}

void readMsg()
{
	int retcode;

	retcode=msgrcv(queueID,&buffer,MAX_MSG_SIZE,msgType,receiveType);

	if(retcode>1)
		printf("%s\n",buffer.mText);
}

void removeQ()
{
	msgctl(queueID,IPC_RMID,0);
}

int main(int argc, char **argv)
{
	int c;
	int key;

	buffer.mType=msgType;
	buffer.mText[0]=0;
	key=0xdeadbeef;

	while (1)
		{
		int option_index = 0;
		c = getopt_long (argc, argv, "v?hdrws:t:k:",long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
			{
			case 's':
				strcpy(buffer.mText,optarg);
				action=true;
				break;
			case 'r':
				action=false;
				break;
			case 't':
				msgType=atoi(optarg);
				break;
		
			case 'd':
				removeQ();
				return ALLOK;
				break;

			case 'w':
				receiveType=WAIT_MSG;
				break;

			case 'k':
				key=atoi(optarg);
				break;

			case 'v':
				printf("climsg %s\n",VERSION);
				return ALLOK;
				break;

			case '?':
			case 'h':
				printHelp();
				return ALLOK;
				break;

			default:
				fprintf(stderr,"?? Unknown argument ??\n");
				return UNKNOWNARG;
			break;
			}
		}

	if((queueID=msgget(key,IPC_CREAT|0660))==-1)
		{
			fprintf(stderr,"Can't create message queue\n");
			exit(NOMAKEQUEUE);
		}

	buffer.mType=msgType;
	if(action==true)
		sendMsg();
	else
		readMsg();

	return(ALLOK);
	
}

