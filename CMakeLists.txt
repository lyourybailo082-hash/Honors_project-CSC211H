cmake_minimum_required(VERSION 3.16)
project(StudentSupportNavigator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(StudentSupportNavigator
    main.cpp
    MainWindow.cpp
    Student.cpp
    StudentManager.cpp
)

target_include_directories(StudentSupportNavigator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(StudentSupportNavigator PRIVATE Qt6::Widgets)
