#include "message.hh"
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

        const char *readFromFile = "shared1.dat";
	const char *writeToFile = "shared2.dat";
        const mode_t mode = 0666;
        const int openFlags = (O_RDWR);
        int fd1 = open(writeToFile, openFlags, mode);
	int fd2 = open(readFromFile, openFlags, mode);
        if(fd1 == (-1) || fd2 == (-1)) {
                printf("open returned (-1)\n");
                return (-1);
        }

        Message* read = Message::GetFromMemoryMappedFile(fd2);
	Message* write = Message::GetFromMemoryMappedFile(fd1);

        int count = 0;

      

        Message::ReleaseFile(read, fd2);
	Message::ReleaseFile(write, fd1);
        close(fd1);
	close(fd2);
}

void Read() {
	 while(1) {
                char *message = read->DequeueMessage();
                printf("%d: %s", count, message);
                fflush(stdout);
        }
}

