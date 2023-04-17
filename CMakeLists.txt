cmake_minimum_required(VERSION 3.5)

project(semaphore)

file(GLOB source_files "./*.c")
add_executable(semaphore ${source_files})

file(GLOB queue_files "./queue/*.c")
add_library(queue ${queue_files})
target_include_directories(queue PRIVATE "./include")

target_include_directories(semaphore PRIVATE "./include")
target_link_libraries(semaphore PRIVATE pthread)
target_link_libraries(semaphore PRIVATE queue)