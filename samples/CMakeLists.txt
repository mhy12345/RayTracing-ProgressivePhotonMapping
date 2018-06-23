

add_executable(eigen_rotation eigen_rotation.cpp)

add_executable(bazier_curve bazier_curve.cpp)
target_link_libraries(bazier_curve ${OpenCV_LIBS})
