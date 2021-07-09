cmake_minimum_required(VERSION 3.0)

set(projectName makelist)

project(${projectName} VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_COMPILER gсс)
set(CMAKE_CXX_COMPILER g++)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(${projectName} 
main.cpp
condition_from_args.cpp
scan_dir.cpp
)

target_include_directories(${projectName} PUBLIC  ${Qt5_INCLUDE_DIR} )
target_link_libraries(${projectName} Qt5::Core)
