cmake_minimum_required(VERSION 3.13)
project(ex2)

set(CMAKE_C_STANDARD 99)

include_directories(ex2)

add_executable(ex2
        ex2/Macros.h
        ex2/communication/Message.c
        ex2/communication/Message.h
        ex2/communication/FileMethod.c
        ex2/communication/FileMethod.h
        ex2/communication/SocketMethod.c
        ex2/communication/SocketMethod.h
        ex2/communication/CommunicationMethods.c
        ex2/communication/CommunicationMethods.h
        ex2/server/Server.c
        ex2/server/Server.h
        ex2/client/Client.c
        ex2/client/Client.h
        ex2/Main.c)
