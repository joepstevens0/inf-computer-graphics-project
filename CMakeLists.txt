cmake_minimum_required (VERSION 3.7)
project (ComputerGraphicsProject)

find_package(Qt5Widgets)
find_package(Qt5Gui)

# Tell CMake to run moc and ui generation when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(ComputerGraphicsProject
        camera.cpp
        camera.h
        glwidget.cpp
        glwidget.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        model.cpp
        model.h)

set(CMAKE_DEFAULT_STARTUP_PROJECT ComputerGraphicsProject)
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "ComputerGraphicsProject")

target_include_directories(ComputerGraphicsProject PUBLIC)
target_link_libraries(ComputerGraphicsProject Qt5::Widgets Qt5::Gui)