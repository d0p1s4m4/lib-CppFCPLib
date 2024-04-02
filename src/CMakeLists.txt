set(SRCS
    Base64.cpp
    Exceptions.cpp
    JobTicket.cpp
    Log.cpp
    Message.cpp
    Node.cpp
    NodeThread.cpp
    Server.cpp
    ServerMessage.cpp
    sha256.cc)

add_library(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE  ${Boost_LIBRARIES} ZThread)