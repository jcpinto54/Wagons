cmake_minimum_required(VERSION 3.16)
project(MeatWagons)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(MeatWagons
        src/Date/Date.cpp
        src/Date/Date.h
        src/Utilities/InvalidInput.h
        src/main.cpp
        src/Menu/Menu.cpp
        src/Menu/Menu.h
        src/Person/Person.cpp
        src/Person/Person.h
        src/System/System.cpp
        src/System/System.h
        src/Utilities/Table.h
        src/Time/Time.cpp
        src/Time/Time.h
        src/Utilities/utils.cpp
        src/Utilities/utils.h)
