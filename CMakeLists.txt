cmake_minimum_required(VERSION 3.8)
project(ex2)
set(CMAKE_C_STANDARD 99)

set (warnings "-Wall -Wextra -Werror")

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL")
set(OPENSSL_INCLUDE_DIR "C:/Program Files/OpenSSL/include")
set(OPENSSL_LIBRARIES "C:/Program Files/OpenSSL/lib")


include_directories(ex2 C:/Program Files/OpenSSL/include/openssl)
find_package(OpenSSL REQUIRED)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message(STATUS "OpenSSL NOT FOUND")
endif()

add_executable(ex2-server
        ex2/Macros.h
        ex2/utils/base64.c
        ex2/utils/base64.h

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
        ex2/utils/base64.c
        ex2/utils/base64.h

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

link_libraries(crypto)

if(WIN32)
    target_link_libraries(ex2-server wsock32 ws2_32)
    target_link_libraries(ex2-client wsock32 ws2_32)
endif()


message( STATUS "${Green}Found OpenSSL library at: ${ssl_LIBRARY}${Reset}" )
message( STATUS "${Green}Found Crypto library at: ${crypto_LIBRARY}${Reset}" )
message( STATUS "${Green}Found OpenSSL include at: ${ssl_INCLUDE}${Reset}" )

