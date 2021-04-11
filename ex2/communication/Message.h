#pragma once

/********************************************************************************
 * @brief A message formatter.
 *
 *
 * @section DESCRIPTION
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/

// ------------------------------ includes ------------------------------

#include "../Macros.h"

// -------------------------- const definitions -------------------------

/*
 * Message represents our format for communicating.
 */
struct _Message;
typedef struct _Message Message;


// ------------------------------ functions -----------------------------

/**
 * @brief allocates a new msg.
 *
 * RETURN VALUE:
 * @return a pointer to the new msg, or NULL if the allocation failed.
 */
Message *messageAllocate();
void messageFree(Message * msg);

// setter
ReturnValue messageSet(Message * msg, Command commandType, Sender sender,
                        unsigned int length, char * contents);

// getters
Command messageGetCommand(Message * msg);
Sender messageGetSender(Message * msg);
unsigned int messageGetLength(Message * msg);
char *messageGetContents(Message * msg);

//  char *   <-->  msg
char *messageToCString(Message * msg);
ReturnValue messageFromCString(Message * msg, const char * cStr);
//TODO think about how a msgs's char * will look like
