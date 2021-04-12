CM_INC_DIR=communicationMethods/include
CM_SRC_DIR=communicationMethods/src
CP_INC_DIR=communicationProtocol/include
CP_SRC_DIR=communicationProtocol/src
SERVER_DIR=server
CLIENT_DIR=client

CC=gcc
CC_FLAGS=-std=c99

OBJ_DIR=objects

onlyServer: communicationMethodObjects communicationObjects serverObjects
	$(CC) $(CC_FLAGS) $? -o serverMain

onlyClient: communicationMethodObjects communicationObjects clientObjects
	$(CC) $(CC_FLAGS) $? -o clientMain

communicationMethodObjects: $(OBJ_DIR)/FileMethod.o

communicationObjects: $(OBJ_DIR)/CommunicationMethods.o 	\
                      $(OBJ_DIR)/Message.o				\
                      $(OBJ_DIR)/MessageProtocol.o

serverObjects: $(OBJ_DIR)/Server.o				\
	 		   $(OBJ_DIR)/ServerMain.o

clientObjects: $(OBJ_DIR)/Client.o				\
	 		   $(OBJ_DIR)/ClientMain.o


$(OBJ_DIR)/ServerMain.o: $(SERVER_DIR)/ServerMain.c $(SERVER_DIR)/Server.h
	$(CC) $(CC_FLAGS) $(SERVER_DIR)/ServerMain.c -c

$(OBJ_DIR)/Server.o: $(SERVER_DIR)/Server.c $(SERVER_DIR)/Server.h $(CP_INC_DIR)/MessageProtocol.h
	$(CC) $(CC_FLAGS) $(SERVER_DIR)/Server.c -c

$(OBJ_DIR)/MessageProtocol.o: $(CP_SRC_DIR)/MessageProtocol.c $(CP_INC_DIR)/MessageProtocol.h $(CM_INC_DIR)/CommunicationMethods.h
	$(CC) $(CC_FLAGS) $(CP_SRC_DIR)/MessageProtocol.c -c

$(OBJ_DIR)/Message.o: $(CP_SRC_DIR)/Message.c $(CP_INC_DIR)/Message.h
	$(CC) $(CC_FLAGS) $(CP_SRC_DIR)/Message.c -c

$(OBJ_DIR)/CommunicationMethods.o: $(CM_SRC_DIR)/CommunicationMethods.c $(CM_INC_DIR)/CommunicationMethods.h $(CM_INC_DIR)/FileMethod.h
	$(CC) $(CC_FLAGS) $(CM_SRC_DIR)/CommunicationMethods.c -c

$(OBJ_DIR)/FileMethod.o: $(CM_SRC_DIR)/FileMethod.c $(CM_INC_DIR)/FileMethod.h
	$(CC) $(CC_FLAGS) $(CM_SRC_DIR)/FileMethod.c -c


.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o