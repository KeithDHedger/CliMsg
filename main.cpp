/*
 *
 * ©K. D. Hedger. Tue 31 Jul 13:14:35 BST 2018 keithdhedger@gmail.com

 * This file(main.cpp) is part of CliMsg.

 * CliMsg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CliMsg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Projects.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <getopt.h>

#define APPNAME "climsg"
#define VERSION "0.1.2"

#define MAX_MSG_SIZE 1024

#define ALLOK 0
#define UNKNOWNARG 1
#define NOMAKEQUEUE 2
#define NOSENDMSG 3
#define WAIT_MSG 0

struct option long_options[] =
{
	{"send",1,0,'s'},
	{"repeat",0,0,'r'},
	{"all",0,0,'a'},
	{"type",1,0,'t'},
	{"key",1,0,'k'},
	{"wait",0,0,'w'},
	{"flush",0,0,'f'},
	{"permissions",1,0,'p'},
	{"version",0,0,'v'},
	{"help",0,0,'?'},
	{0, 0, 0, 0}
};

struct msgBuffer
{
	long		mType;
	char		mText[MAX_MSG_SIZE];
};

int			queueID;
msgBuffer	buffer;
int			msgType=1;
bool			action=false;
int			receiveType=IPC_NOWAIT;
bool			repeat=false;
bool			printAll=false;
bool			allDone=false;
bool			flushQueue=false;
int			setPerms=0660;

void printHelp()
{
	printf("Usage: %s [OPTION] [TEXT]\n"
	       "A CLI application for inter-process communication\n"
	       " -s, --send		Send message [TEXT](defaults to receive)\n"
	       " -r, --repeat		Print received message and resend\n"
	       " -a, --all		Print all messages in queue\n"
	       " -f, --flush		Flush message queue quietly\n"
	       " -t, --type		Message type(defaults to 1)\n"
	       " -k, --key		Use key [INTEGER] instead of generated one\n"
	       " -w, --wait		Wait for message to arrive(blocking)\n"
	       " -p, --permissions	Set perms on queue( default 0660 )\n"
	       " -v, --version		output version information and exit\n"
	       " -h, -?, --help		print this help\n\n"
	       "Report bugs to keithdhedger@gmail.com\n"
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
	else
		allDone=true;
}

int main(int argc, char **argv)
{
	int c;
	int key;
	int retcode;

	buffer.mType=msgType;
	buffer.mText[0]=0;
	key=0xdeadbeef;

	while(1)
		{
			int option_index=0;
			c=getopt_long(argc, argv, "v?hewfras:t:k:p:",long_options, &option_index);
			if(c == -1)
				break;

			switch(c)
				{
				case 'p':
					setPerms=atoi(optarg);
					break;
				case 's':
					strcpy(buffer.mText,optarg);
					action=true;
					break;

				case 'r':
					repeat=true;
					break;

				case 'a':
					printAll=true;
					break;

				case 'f':
					flushQueue=true;
					break;

				case 't':
					msgType=atoi(optarg);
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
//IPC_EXCL
	if((queueID=msgget(key,IPC_CREAT|setPerms))==-1)
		{
			fprintf(stderr,"Can't create message queue\n");
			exit(NOMAKEQUEUE);
		}

	buffer.mType=msgType;

	if(printAll==true)
		{
			while(allDone==false)
				{
					readMsg();
				}
			return(ALLOK);
		}

	if(flushQueue==true)
		{
			allDone=false;
			msgType=0;
			while(allDone==false)
				{
					retcode=msgrcv(queueID,&buffer,MAX_MSG_SIZE,msgType,receiveType);
					if(retcode<=1)
						allDone=true;
				}
			return(ALLOK);
		}

	if(action==true)
		sendMsg();
	else
		{
			readMsg();
			if(repeat==true)
				sendMsg();
		}

	return(ALLOK);
}

