#include "message.hh"
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

        const char *writeToFile = "shared1.dat";
	const char *readFromFile = "shared2.dat";
        const mode_t mode = 0666;
        const int openFlags = (O_CREAT | O_TRUNC | O_RDWR);
        int fd1 = open(writeToFile, openFlags, mode);
	int fd2 = open(readFromFile, openFlags, mode);
        if(fd1 == (-1) || fd2 == (-1)) {
                printf("open returned (-1)\n");
                return (-1);
        }

        
	

        for(int i = 0; i < 100; i++) {
                char message[10];
                sprintf(message, "%d\n", i);
                write->EnqueueMessage(&message[0]);
                printf("enqueued %d\n", i);
        }

        printf("message queue written\n");
        getchar();

        Message::ReleaseFile(read, fd2);
	Message::ReleaseFile(write, fd1);
        close(fd1);
	close(fd2);
}

