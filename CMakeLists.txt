cmake_minimum_required(VERSION 3.8)
project(ex2)

set(CMAKE_C_STANDARD 99)
set(warnings "-Wall -Wextra -Werror")

include_directories(ex2 ex2/client ex2/server)
include_directories(ex2/communicationMethods/include ex2/communicationProtocol/include)
include_directories(ex2/utils/include)

add_executable(ex2-server
        ex2/Macros.h
        ex2/utils/src/base64.c
        ex2/utils/include/base64.h
        ex2/utils/src/config.c
        ex2/utils/include/config.h
        ex2/utils/src/nxjson.c
        ex2/utils/include/nxjson.h

        ex2/communicationProtocol/src/Message.c
        ex2/communicationProtocol/include/Message.h
        ex2/communicationProtocol/src/MessageProtocol.c
        ex2/communicationProtocol/include/MessageProtocol.h

        ex2/communicationMethods/src/FileMethod.c
        ex2/communicationMethods/include/FileMethod.h
        ex2/communicationMethods/src/SocketMethod.c
        ex2/communicationMethods/include/SocketMethod.h
        ex2/communicationMethods/src/CommunicationMethods.c
        ex2/communicationMethods/include/CommunicationMethods.h

        ex2/server/Server.c
        ex2/server/Server.h
        ex2/server/ServerMain.c)

add_executable(ex2-client
        ex2/Macros.h
        ex2/utils/src/base64.c
        ex2/utils/include/base64.h
        ex2/utils/src/config.c
        ex2/utils/include/config.h
        ex2/utils/src/nxjson.c
        ex2/utils/include/nxjson.h

        ex2/communicationProtocol/src/Message.c
        ex2/communicationProtocol/include/Message.h
        ex2/communicationProtocol/src/MessageProtocol.c
        ex2/communicationProtocol/include/MessageProtocol.h

        ex2/communicationMethods/src/FileMethod.c
        ex2/communicationMethods/include/FileMethod.h
        ex2/communicationMethods/src/SocketMethod.c
        ex2/communicationMethods/include/SocketMethod.h
        ex2/communicationMethods/src/CommunicationMethods.c
        ex2/communicationMethods/include/CommunicationMethods.h

        ex2/client/Client.c
        ex2/client/Client.h
        ex2/client/ClientMain.c)

if(WIN32)
    target_link_libraries(ex2-server wsock32 ws2_32)
    target_link_libraries(ex2-client wsock32 ws2_32)
endif()
