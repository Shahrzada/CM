CM_INC_DIR=communicationMethods/include
CM_SRC_DIR=communicationMethods/src
CP_DIR=communicationProtocol/include
SERVER_DIR=server
CLIENT_DIR=client
CC=gcc

OBJ_DIR=objects

#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(COMMUNICATION_DIRECTORY)/%,$(_DEPS))
#
#_OBJ = server.o client.o
#OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

main: $(OBJ_DIR)/Main.o $(OBJ_DIR)/CommunicationMethods.o $(OBJ_DIR)/FileMethod.o
	$(CC) $(OBJ_DIR)/Main.o $(OBJ_DIR)/CommunicationMethods.o $(OBJ_DIR)/FileMethod.o -o main

$(OBJ_DIR)/Main.o: Main.c $(CM_INC_DIR)/CommunicationMethods.h
	$(CC) -c Main.c

$(OBJ_DIR)/CommunicationMethods.o: $(CM_SRC_DIR)/CommunicationMethods.c $(CM_INC_DIR)/CommunicationMethods.h $(CM_INC_DIR)/FileMethod.h
	$(CC) -c $(CM_SRC_DIR)/CommunicationMethods.c

$(OBJ_DIR)/FileMethod.o: $(CM_SRC_DIR)/FileMethod.c $(CM_INC_DIR)/FileMethod.h
	$(CC) -c $(CM_SRC_DIR)/FileMethod.c


.PHONY: clean

clean:
	rm edit $(objects)