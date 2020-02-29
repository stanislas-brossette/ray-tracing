cmake_minimum_required(VERSION 2.8.9)
project (RayTracing)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

include_directories(include)
#set(SOURCES src/main.cc src/Snake.cc)

file(GLOB SOURCES "src/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package (Threads)
find_package(Curses REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${EXTRA_LIBS}
)
