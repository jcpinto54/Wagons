cmake_minimum_required(VERSION 3.16)
project(MeatWagons)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MeatWagons
        Date/Date.cpp
        Date/Date.h
        Utilities/InvalidInput.h
        main.cpp
        Menu/Menu.cpp
        Menu/Menu.h
        Person/Person.cpp
        Person/Person.h
        System/System.cpp
        System/System.h
        Utilities/Table.h
        Time/Time.cpp
        Time/Time.h
        Utilities/utils.cpp
        Utilities/utils.h
        Graph/graphviewer.cpp
        Graph/graphviewer.h
        Graph/connection.cpp
        Graph/connection.h
        Graph/Graph.h
        Graph/MutablePriorityQueue.h
        Local/Local.h
        Local/Local.cpp)
