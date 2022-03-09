include(GoogleTest)
enable_testing()

# VTK Window
add_executable(test_vtk_window test_vtk_window.cxx test_utils.h)
target_link_libraries(test_vtk_window PRIVATE GTest::gtest_main PUBLIC viz3d)
gtest_discover_tests(test_vtk_window)