cmake_minimum_required(VERSION 2.8.12)

project(qt-opencascade-experiment)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
include_directories(${OpenCASCADE_INCLUDE_DIR})

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB_RECURSE HEADERS "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCASCADE_FoundationClasses_LIBRARIES} ${OpenCASCADE_ModelingData_LIBRARIES} ${OpenCASCADE_ModelingAlgorithms_LIBRARIES})
