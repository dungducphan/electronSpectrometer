cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(fission)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACROS "*.mac" "*.C" "*.sh" "*.csv")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${EXT_DATA} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(fission sim.cc ${sources} ${headers})
target_link_libraries(fission ${Geant4_LIBRARIES})
target_link_libraries(fission ${ROOT_LIBRARIES})
