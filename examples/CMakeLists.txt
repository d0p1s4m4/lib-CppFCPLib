add_executable(blocking_tcp_echo_server message_sending/blocking_tcp_echo_server.cpp)
target_link_libraries(blocking_tcp_echo_server PRIVATE ${PROJECT_NAME})

#add_executable(send_message message_sending/send_message.cpp)
#target_link_libraries(send_message PRIVATE ${PROJECT_NAME})

add_executable(list_peer_notes list_peer_notes.cpp)
target_link_libraries(list_peer_notes PRIVATE ${PROJECT_NAME})

add_executable(list_peers list_peers.cpp)
target_link_libraries(list_peers PRIVATE ${PROJECT_NAME})