cmake_minimum_required(VERSION 3.25)
project(tic_tac_toe)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Find source and headers using glob
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "inc/*.hpp")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

# Link Qt libraries
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME}  Qt6::Core Qt6::Gui Qt6::Widgets)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC inc)
