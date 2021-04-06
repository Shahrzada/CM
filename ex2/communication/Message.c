#include "Message.h"

struct _Message {
    Command commandType;
    Sender sender;
    unsigned int length;
    char * contents;
    /* time stamp? */
};
