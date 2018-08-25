cmake_minimum_required(VERSION 2.8.12)

project(qt-opencascade-experiment)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DGLM_FORCE_SWIZZLE=1)
add_definitions(-D_USE_MATH_DEFINES)

find_package(Qt5Widgets)
find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
link_directories(${OpenCASCADE_LIBRARY_DIR})
if (WIN32)
    link_directories("C:/OpenCASCADE-7.3.0-vc14-64/tbb_2017.0.100/lib/intel64/vc14") #TODO: avoid hard conding tbb path
endif(WIN32)
include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories("vendor/glm")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB_RECURSE HEADERS "src/*.hpp")
qt5_add_resources(RESOURCES "src/resources.qrc")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCASCADE_FoundationClasses_LIBRARIES} ${OpenCASCADE_ModelingData_LIBRARIES} ${OpenCASCADE_ModelingAlgorithms_LIBRARIES})
