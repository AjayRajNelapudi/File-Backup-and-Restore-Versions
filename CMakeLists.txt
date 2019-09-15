cmake_minimum_required(VERSION 3.14)
project(FileBackupandRestore)

set(CMAKE_CXX_STANDARD 14)

add_executable(FileBackupandRestore main.cpp diskutilities.cpp diskutilities.h backupandrestore.cpp backupandrestore.h)