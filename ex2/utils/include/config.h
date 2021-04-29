#pragma once

#include "Macros.h"

#define MAX_JSON_VALUE_LENGTH 100

ReturnValue initConfigurations(const char *configFilePath);

CommunicationMethodCode getCommunicationMethodCode();
char *getFileToTransferPath();
char *getServerCommunicationFilePath();
char *getServerTempCommunicationFilePath();
char *getClientCommunicationFilePath();
char *getClientTempCommunicationFilePath();
char *getServerLogCommunicationFilePath();
char *getClientLogCommunicationFilePath();
