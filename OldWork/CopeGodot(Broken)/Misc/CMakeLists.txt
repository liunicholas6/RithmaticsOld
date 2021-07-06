cmake_minimum_required(VERSION 3.12)

project(Rithmatist)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# set(CMAKE_CXX_COMPILER g++)
# set(CMAKE_C_COMPILER gcc)

#set(CMAKE_CXX_FLAGS "-lpthread -lm")
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


add_compile_options(-Wno-deprecated-enum-enum-conversion -Wno-comment)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

# Include dependancies
# set(CMAKE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dep)

# set(NUKLEAR_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/dep)

# put the binaries where I want them
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_DEBUG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)

# stuff for debugging
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
#set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# To make everything fully multithreaded compliant
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# gcc -o test test.c sdl-config --cflags --libs

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SRC_LIST
    ${CMAKE_SOURCE_DIR}/src/Arena.cpp
    ${CMAKE_SOURCE_DIR}/src/Gui.cpp
    ${CMAKE_SOURCE_DIR}/src/Line.cpp
    ${CMAKE_SOURCE_DIR}/src/StdMathFunc.cpp
    ${CMAKE_SOURCE_DIR}/src/Geometry.cpp
    ${CMAKE_SOURCE_DIR}/src/Globals.cpp
    ${CMAKE_SOURCE_DIR}/src/Point.cpp
    ${CMAKE_SOURCE_DIR}/src/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Player.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    # ${CMAKE_SOURCE_DIR}/src/Nuklear.cpp
    # ${CMAKE_SOURCE_DIR}/src/nuklear.h
    # ${CMAKE_SOURCE_DIR}/src/nuklear_sdl_gl3.h
)

# set(DEP_LIST
#     ${CMAKE_SOURCE_DIR}/src/nuklear.h
#     ${CMAKE_SOURCE_DIR}/src/nuklear_sdl_gl3.h
# )

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    # ${NUKLEAR_INCLUDE_DIR}
)

set(BINARY_NAME "${PROJECT_NAME}-${CMAKE_BUILD_TYPE}")

add_executable(${BINARY_NAME} ${SRC_LIST})

#link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(${BINARY_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} dl)
#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
