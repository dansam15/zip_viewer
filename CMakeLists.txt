cmake_minimum_required(VERSION 3.5)

project(zip_viewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17) # 17 for std::filesystem
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(minizip-ng/)
add_executable(zip_viewer
    treemodel.cpp
    treeitem.cpp
    main.cpp
     )

target_link_libraries(zip_viewer PRIVATE minizip Qt5::Widgets)
