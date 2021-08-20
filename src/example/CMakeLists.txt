# Build Test Executables
add_executable(example example.cpp)
target_link_libraries(example PUBLIC viz3d)
install(TARGETS example RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
