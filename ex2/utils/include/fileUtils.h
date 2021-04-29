#pragma once

#include "Macros.h"

char *loadFileAsString(const char *configFilePath, unsigned int maxBufferSize);

ReturnValue getFileTitleAndExtension(char *filePath, unsigned int filePathLength, char *buffer);
