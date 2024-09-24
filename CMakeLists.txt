cmake_minimum_required(VERSION 3.24)
project(Note_task_job)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIR})

add_executable(Note_task_job main.cpp DatabaseManager.cpp DatabaseManager.h Note.cpp Note.h NoteManager.cpp NoteManager.h Application.cpp Application.h
        NoteSearchManager.cpp
        NoteSearchManager.h)

target_link_libraries(Note_task_job ${SQLite3_LIBRARIES})
