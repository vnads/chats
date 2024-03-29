#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <semaphore.h>

const int MessageQueueSize = (500);
const int MaxMessageSize = (20);

class Message {
public:
        ~Message();
        void EnqueueMessage(const char *msg);
        char* DequeueMessage();
        static Message *CopyToMemoryMappedFile(int fd);
        static Message *GetFromMemoryMappedFile(int fd);
        static void ReleaseFile(Message *msg, int fd);
private:
        Message();
        sem_t _lock;
        sem_t _empty;
        sem_t _full;
        char _messages[MessageQueueSize][MaxMessageSize];
	int _current;
};

#endif

