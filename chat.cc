#include "message.hh" 
#include <stdio.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <fstream> 
#include <iostream> 
#include <pthread.h>
#include <string.h>

void *CheckForMessages(void* ptr) {
	const char *readFile;
	readFile = (char*)ptr;
	const mode_t mode = 0666;
	const int openFlags = (O_RDWR);
	int fdread = open(readFile, openFlags, mode);
	
	if(fdread == (-1)) {
		printf("reader open returned (-1)\n");
		return NULL;
	}

	Message* read = Message::GetFromMemoryMappedFile(fdread);
	while(1) {
        char *message = read->DequeueMessage();
        printf("Other Dude: %s\n", message);
       	fflush(stdout);
    }

	Message::ReleaseFile(read, fdread);
		
    close(fdread);
}


void *WriteMessage(void* ptr) {

	const char *writeFile;
	writeFile = (char*)ptr;
	const mode_t mode = 0666;
	const int openFlags = (O_RDWR);
	int fdwrite = open(writeFile, openFlags, mode);
	
	if(fdwrite == (-1)) {
		printf("reader open returned (-1)\n");
		return NULL;
	}
	Message* write = Message::GetFromMemoryMappedFile(fdwrite);
	while(1) {
		std::string str;
		getline(std::cin, str);
	    char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		write->EnqueueMessage(cstr);	
	}

	Message::ReleaseFile(write, fdwrite);
	close(fdwrite); 
}

int main(int argc, char* argv[]) {
	const mode_t mode = 0666;
    const int openFlags = (O_CREAT | O_TRUNC | O_RDWR);
	
	char *readFromFile;	
	char *writeToFile;

	if(!strcmp(argv[1], "1")) {
		readFromFile = "shared2.dat";
		writeToFile = "shared1.dat";
		
		//create the fileS
		int fdread = open(readFromFile, openFlags, mode);
		int fdwrite = open(writeToFile, openFlags, mode);
	
		if(fdread == (-1)) {
			printf("reader open returned (-1)\n");
			return (-1);
		}

		
		if(fdwrite == (-1)) {
			printf("writer open returned (-1)\n");
			return (-1);
		}

		Message* write = Message::CopyToMemoryMappedFile(fdwrite);
		Message* read = Message::CopyToMemoryMappedFile(fdread);

		Message::ReleaseFile(read, fdread);		
		close(fdread);

		Message::ReleaseFile(write, fdwrite);
		close(fdwrite);
	
	}
	else {
		readFromFile = "shared1.dat";
		writeToFile = "shared2.dat";
	}	                    		

	
	
	pthread_t readThread;
	int readThreadresult; 
	pthread_t writeThread;
	int writeThreadresult;

    readThreadresult = pthread_create(&readThread, NULL, &CheckForMessages, (void*)readFromFile);
	writeThreadresult = pthread_create(&writeThread, NULL, &WriteMessage, (void*)writeToFile);

	pthread_join(readThread, NULL);
	pthread_join(writeThread, NULL);
	

    
	
}


