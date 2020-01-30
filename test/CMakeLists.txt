add_executable(server server.cpp)
add_executable(client client.cpp)
target_link_libraries(server smartwallet_lib)
target_link_libraries(client smartwallet_lib)
