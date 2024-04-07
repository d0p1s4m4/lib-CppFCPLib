add_executable(hello hello.cc)
target_link_libraries(hello PRIVATE ${PROJECT_NAME})