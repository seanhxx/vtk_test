#TODO: Add cross platform compatibiliy

###########
# Project #
###########
cmake_minimum_required(VERSION 3.10)

project(VTKTEST LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#########################
# External Dependencies #
#########################

#Set all dependencies dir
#Create Environment Variable QT_DIR, VTK_DIR point to bin. Example:
set(VTK_INSTALL_PREFIX $ENV{VTK_DIR}/../)
set(CMAKE_PREFIX_PATH "${VTK_INSTALL_PREFIX}")

#Find VTK
find_package(VTK)
include(${VTK_USE_FILE})

#########
# BUILD #
#########
set(SOURCES
    src/main.cpp
)

#Include QObject class header files here
set(HEADERS
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:interface>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

########
# LINK #
########
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_11)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/RelWithDebInfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin/MinSizeRel"
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PUBLIC
        ${VTK_LIBRARIES}
)

##############
# POST BUILD #
##############

file(GLOB REVTK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/ironProt.vtk")
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${REVTK_PATH}" 
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
)
