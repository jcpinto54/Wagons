cmake_minimum_required(VERSION 3.15)
project(MeatWagons)

set(CMAKE_CXX_STANDARD 11)

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
#        src/Person/Prisioner.cpp
#        src/Person/Prisioner.h
        )
