cmake_minimum_required(VERSION 3.5)

project(zip_viewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fpie")
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

#if()
#else()
set(MZ_BZIP2=FALSE)
set(MZ_LZMA=FALSE)
set(MZ_ZLIB=FALSE)

  add_subdirectory(minizip-ng/)

  add_executable(zip_viewer
    treemodel.cpp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
     )

  #target_include_directories(zip_viewer PRIVATE minizip-ng/)
  target_link_libraries(zip_viewer PRIVATE minizip)
#endif()

target_link_libraries(zip_viewer PRIVATE Qt5::Widgets)
