#pragma once

/* Communication methods */
typedef enum {
    SOCKET_METHOD = 0,
    FILE_METHOD,
} CommunicationMethodCode;

/* Return values */
typedef enum {
    SUCCESS = 0,
    ERROR = -999,
} ReturnValue;

/* Command types */
typedef enum {
    READ = 0,
    WRITE,
    SCHNITZEL,
} Command;

/* Sender types */
typedef enum {
    SERVER = 0,
    CLIENT,
} Sender;

