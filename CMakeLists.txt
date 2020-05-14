cmake_minimum_required(VERSION 3.16)
project(MeatWagons)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(MeatWagons
        src/Date.cpp
        src/Date.h
        src/InvalidInput.h
        src/main.cpp
        src/Menu.cpp
        src/Menu.h
        src/Person.cpp
        src/Person.h
        src/System.cpp
        src/System.h
        src/Table.h
        src/Time.cpp
        src/Time.h
        src/utils.cpp
        src/utils.h)
