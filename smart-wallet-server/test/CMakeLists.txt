add_executable(server server.cpp)
target_link_libraries(server smartwallet_lib)

# MySQL
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")    
find_package(MySQL)
target_link_libraries(server ${MYSQL_LIBRARY})

# threads
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(server PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(server "${CMAKE_THREAD_LIBS_INIT}")
endif()
