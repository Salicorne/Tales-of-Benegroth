cmake_minimum_required(VERSION 3.11)

project(MyProject)

set (CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_NAME "app")

file(GLOB_RECURSE sources Source/*.cpp Source/*.h)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_executable(${EXECUTABLE_NAME} ${sources})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  target_compile_options(${EXECUTABLE_NAME} PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${EXECUTABLE_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()

find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
