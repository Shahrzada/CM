#pragma once

#include "Macros.h"

#define MAX_JSON_VALUE_LENGTH 100

ReturnValue initConfigurations(const char *configFilePath);

CommunicationMethodCode getCommunicationMethodCode();
const char *getFileToTransferPath();
const char *getServerCommunicationFilePath();
const char *getServerTempCommunicationFilePath();
const char *getClientCommunicationFilePath();
const char *getClientTempCommunicationFilePath();
