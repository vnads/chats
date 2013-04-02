#include "message.hh"
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

void *CheckForMessages(void* ptr) {
	Message* msg;
	msg = (Message*)ptr;
	int count = 0;
	while(1) {
        char *message = msg->DequeueMessage();
        printf("Other Dude: %s", message);
        fflush(stdout);
    }
}

int main(int argc, char* argv[]) {
	const mode_t mode = 0666;
    const int openFlags = (O_CREAT | O_TRUNC | O_RDWR);
	
	int fdread = 0;
	int fdwrite = 0;
	Message* read;
	Message* write;

	if(!strcmp(argv[1], "1")) {
		const char *readFromFile = "shared2.dat";
		const char *writeToFile = "shared1.dat";
		fdwrite = open(writeToFile, openFlags, mode);
		fdread = open(readFromFile, openFlags, mode);
		printf("%d\n", fdwrite);
		printf("%d\n", fdread);
		if(fdread == (-1) || fdwrite == (-1)) {
			printf("open returned (-1) copy\n");
			return (-1);
		}
		write = Message::CopyToMemoryMappedFile(fdwrite);
		read = Message::CopyToMemoryMappedFile(fdread);
	}
	else {
		const char *readFromFile = "shared1.dat";
		const char *writeToFile = "shared2.dat";
		fdwrite = open(writeToFile, openFlags, mode);
		fdread = open(readFromFile, openFlags, mode);
		if(fdread == (-1) || fdwrite == (-1)) {
			printf("open returned (-1) get\n");
			return (-1);
		}
		read = Message::GetFromMemoryMappedFile(fdread);
		write = Message::GetFromMemoryMappedFile(fdwrite);        

	}	                    		
	
	pthread_t t1;
	int thread; 

    thread = pthread_create(&t1, NULL, &CheckForMessages, (void*)read);
	pthread_join(t1, NULL);

	while(1) {
		const char *msg;
		cin>>msg;
	}

    Message::ReleaseFile(read, fdread);
	Message::ReleaseFile(write, fdwrite);
    close(fdread);
	close(fdwrite);
}

