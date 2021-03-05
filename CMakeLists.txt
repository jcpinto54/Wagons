cmake_minimum_required(VERSION 3.15)
project(Wagons)

set(CMAKE_CXX_STANDARD 11)

#link_libraries(ws2_32 wsock32)

include_directories(src)
add_executable(Wagons
        src/Date/Date.cpp
        src/Date/Date.h
        src/Utilities/InvalidInput.h
        src/main.cpp
        src/Menu/Menu.cpp
        src/Menu/Menu.h
        src/System/System.cpp
        src/System/System.h
        src/Utilities/Table.h
        src/Date/Time.cpp
        src/Date/Time.h
        src/Utilities/utils.cpp
        src/Utilities/utils.h
        src/GraphTemplate/graphviewer.cpp
        src/GraphTemplate/graphviewer.h
        src/GraphTemplate/connection.cpp
        src/GraphTemplate/connection.h
        src/GraphTemplate/Graph.h
        src/GraphTemplate/MutablePriorityQueue.h
        src/Local/Local.h
        src/Local/Local.cpp
        src/Wagon/Wagon.h
        src/Wagon/Wagon.cpp
        src/Map/Map.cpp
        src/Map/Map.h
        src/Local/POI.cpp
        src/Local/POI.h
        src/Date/DateTime.cpp
        src/Date/DateTime.h
        src/Prisioner/Prisioner.cpp
        src/Prisioner/Prisioner.h
        )
