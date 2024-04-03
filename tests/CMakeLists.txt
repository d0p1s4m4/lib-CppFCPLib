add_executable(tests test_base64.cc)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})

catch_discover_tests(tests)