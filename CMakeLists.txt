cmake_minimum_required(VERSION 3.5)

project(zip_viewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17) # 17 for std::filesystem
#set(CMAKE_CXX_FLAGS "-fpie")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address,leak,undefined")

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Widgets REQUIRED)

#if()
#else()
set(MZ_BZIP2=FALSE)
set(MZ_LZMA=FALSE)
set(MZ_ZLIB=FALSE)

  add_subdirectory(minizip-ng/)

  add_executable(zip_viewer
    treemodel.cpp
    treeitem.cpp
    main.cpp
     )

  target_link_libraries(zip_viewer PRIVATE minizip)
#endif()

target_link_libraries(zip_viewer PRIVATE Qt5::Widgets)
