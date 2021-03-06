cmake_minimum_required(VERSION 2.8.9)
project (RayTracing)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

find_package (Threads)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
include_directories(SYSTEM ${SDL2_TTF_INCLUDE_DIRS})
include_directories(SYSTEM ${SDL2_MIXER_INCLUDE_DIRS})
include_directories(SYSTEM ${SDL2_IMAGE_INCLUDE_DIRS})

set(WARN_FULL "-Wall -Wextra -Weffc++ -pedantic -pedantic-errors \
               -Wcast-align -Wcast-qual -Wfloat-equal -Wformat=2 \
               -Wformat-nonliteral -Wformat-security -Wformat-y2k \
               -Wimport -Winit-self -Winvalid-pch -Wlong-long \
               -Wmissing-field-initializers -Wmissing-format-attribute \
               -Wmissing-noreturn -Wpacked -Wpointer-arith \
               -Wredundant-decls -Wshadow -Wstack-protector \
               -Wstrict-aliasing=2 -Wswitch-default -Wswitch-enum \
               -Wunreachable-code -Wunused -Wunused-parameter"
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g ${WARN_FULL}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG ${WARN_FULL} -Wno-deprecated-declarations -Wno-unused-parameter -Wfatal-errors")

option(CMAKE_BUILD_TYPE "Choose the type of build." Release)

# Building Library
set(SRC
    "${CMAKE_SOURCE_DIR}/src/AmbiantLight.cc"
    "${CMAKE_SOURCE_DIR}/src/BitmapWriter.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/BoundingSphere.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/BoundingPolyhedron.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/HierarchyBoundingVolume.cc"
    "${CMAKE_SOURCE_DIR}/src/Camera.cc"
    "${CMAKE_SOURCE_DIR}/src/Frame3.cc"
    "${CMAKE_SOURCE_DIR}/src/InputHandler.cc"
    "${CMAKE_SOURCE_DIR}/src/Item.cc"
    "${CMAKE_SOURCE_DIR}/src/LightRay.cc"
    "${CMAKE_SOURCE_DIR}/src/Material.cc"
    "${CMAKE_SOURCE_DIR}/src/PerformanceTracker.cc"
    "${CMAKE_SOURCE_DIR}/src/Pixel.cc"
    "${CMAKE_SOURCE_DIR}/src/Renderer.cc"
    "${CMAKE_SOURCE_DIR}/src/Scene.cc"
    "${CMAKE_SOURCE_DIR}/src/SceneLoader.cc"
    "${CMAKE_SOURCE_DIR}/src/MotionLoader.cc"
    "${CMAKE_SOURCE_DIR}/src/Vector3.cc"
    "${CMAKE_SOURCE_DIR}/src/Window.cc"
    "${CMAKE_SOURCE_DIR}/src/textures/Texture.cc"
    "${CMAKE_SOURCE_DIR}/src/textures/SolidColor.cc"
    "${CMAKE_SOURCE_DIR}/src/textures/CheckerBoard.cc"
    "${CMAKE_SOURCE_DIR}/src/textures/DiscoBoard.cc"
    "${CMAKE_SOURCE_DIR}/src/textures/Image.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/ClosedCylinder.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Cube.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Cylinder.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Geometry.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Mesh.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Plane.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Polygon.cc"
    "${CMAKE_SOURCE_DIR}/src/geometries/Sphere.cc"
    "${CMAKE_SOURCE_DIR}/src/utils.cc"
    )
add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${EXTRA_LIBS}
)

# Data folder
set(DATA_FOLDER "${CMAKE_SOURCE_DIR}/data/")
add_definitions(-DDATA_FOLDER="${CMAKE_SOURCE_DIR}/data/")
set(MESHES_FOLDER "${CMAKE_SOURCE_DIR}/data/meshes/")
add_definitions(-DMESHES_FOLDER="${CMAKE_SOURCE_DIR}/data/meshes/")
set(IMAGES_FOLDER "${CMAKE_SOURCE_DIR}/data/images/")
add_definitions(-DIMAGES_FOLDER="${CMAKE_SOURCE_DIR}/images/")
set(UNIT_DATA_FOLDER "${CMAKE_SOURCE_DIR}/unit/data/")
add_definitions(-DUNIT_DATA_FOLDER="${CMAKE_SOURCE_DIR}/unit/data/")

# Building executable
set(SRC_MOTION
    "${CMAKE_SOURCE_DIR}/src/mainMotion.cc"
    )
add_executable(Main${PROJECT_NAME} ${SRC_MOTION})
target_include_directories(Main${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(Main${PROJECT_NAME} ${PROJECT_NAME})

set(SRC_VIDEO
    "${CMAKE_SOURCE_DIR}/src/mainVideo.cc"
    )
add_executable(Video${PROJECT_NAME} ${SRC_VIDEO})
target_include_directories(Video${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(Video${PROJECT_NAME} ${PROJECT_NAME})

set(SRC_ROBOT_VIDEO
    "${CMAKE_SOURCE_DIR}/src/mainRobotVideo.cc"
    )
add_executable(RobotVideo${PROJECT_NAME} ${SRC_ROBOT_VIDEO})
target_include_directories(RobotVideo${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(RobotVideo${PROJECT_NAME} ${PROJECT_NAME})

set(SRC_DISCO_VIDEO
    "${CMAKE_SOURCE_DIR}/src/mainDiscoVideo.cc"
    )
add_executable(DiscoVideo${PROJECT_NAME} ${SRC_DISCO_VIDEO})
target_include_directories(DiscoVideo${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(DiscoVideo${PROJECT_NAME} ${PROJECT_NAME})

set(SRC_TRANSITION_VIDEO
    "${CMAKE_SOURCE_DIR}/src/mainDiscoTransitionVideo.cc"
    )
add_executable(TransitionVideo${PROJECT_NAME} ${SRC_TRANSITION_VIDEO})
target_include_directories(TransitionVideo${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(TransitionVideo${PROJECT_NAME} ${PROJECT_NAME})


# Build unit tests
#add_subdirectory("unit")
