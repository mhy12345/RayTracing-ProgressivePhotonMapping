PROJECT(DRAW_POLYGON)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -Wunused-function -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")


add_subdirectory(core)
add_subdirectory(display)
add_subdirectory(render)
add_subdirectory(objects)
add_subdirectory(googletest)
add_subdirectory(jsoncpp)

find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(glog REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main Core_LIBS)
target_link_libraries(main Display_LIBS)
target_link_libraries(main Render_LIBS)
target_link_libraries(main Objects_LIBS)
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main jsoncpp)
target_link_libraries(main glog)


add_executable(unittest unittest.cpp)
target_link_libraries(unittest gtest gmock_main Core_LIBS)
target_link_libraries(unittest jsoncpp)
target_link_libraries(unittest glog)


add_custom_command(TARGET main
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${DRAW_POLYGON_SOURCE_DIR}/configures configures
	COMMENT "Copy the configures into directory..."
	)

