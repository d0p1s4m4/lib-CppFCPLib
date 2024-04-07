set(SRCS
    client.cc)

add_library(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Boost::system)
if(MSVC OR MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 mswsock) # FUCK YOU
endif()