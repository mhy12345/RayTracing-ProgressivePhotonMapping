PROJECT(DRAW_POLYGON)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -Wunused-function -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

add_subdirectory(core)
add_subdirectory(display)
add_subdirectory(googletest)

find_package(OpenCV REQUIRED)


add_executable(main main.cpp)
target_link_libraries(main Core_LIBS)
target_link_libraries(main Display_LIBS)
target_link_libraries(main ${OpenCV_LIBS})

add_executable(unittest unittest.cpp)
target_link_libraries(unittest gtest gmock_main Core_LIBS)


add_custom_command(TARGET main
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${DRAW_POLYGON_SOURCE_DIR}/configures configures
	COMMENT "Copy the configures into directory..."
	)

