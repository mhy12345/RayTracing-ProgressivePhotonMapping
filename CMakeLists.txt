#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#SET(CMAKE_C_COMPILER "/usr/bin/clang")
PROJECT(DRAW_POLYGON)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -Wunused-function -std=c++11 -fopenmp")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11 -DNDEBUG -fopenmp")

find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(glog REQUIRED)
find_package(Eigen3 REQUIRED)

link_directories(/usr/local/opt/llvm/lib)
include_directories(${EIGEN3_INCLUDE_DIR})

add_subdirectory(core)
add_subdirectory(display)
add_subdirectory(render)
add_subdirectory(objects)
add_subdirectory(samples)
#add_subdirectory(jsoncpp)
#add_subdirectory(eigen)



add_executable(main main.cpp)
#target_compile_options(main PUBLIC PUBLIC -fopenmp)
target_link_libraries(main Core_LIBS)
target_link_libraries(main Display_LIBS)
target_link_libraries(main Render_LIBS_omp)
target_link_libraries(main Objects_LIBS)
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main jsoncpp)
target_link_libraries(main glog)

add_executable(serial serial.cpp)
target_link_libraries(serial Core_LIBS)
target_link_libraries(serial Display_LIBS)
target_link_libraries(serial Render_LIBS)
target_link_libraries(serial Objects_LIBS)
target_link_libraries(serial ${OpenCV_LIBS})
target_link_libraries(serial jsoncpp)
target_link_libraries(serial glog)

add_custom_command(TARGET main
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${DRAW_POLYGON_SOURCE_DIR}/configures configures
	COMMENT "Copy the configures into directory..."
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${DRAW_POLYGON_SOURCE_DIR}/materials materials
	COMMENT "Copy the materials into directory..."
	)

