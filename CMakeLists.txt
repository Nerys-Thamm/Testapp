cmake_minimum_required(VERSION 3.0.0)
project(Testapp VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(Testapp/Dependencies/GLEW Testapp/Dependencies/GLFW Testapp/Dependencies/GLM Testapp/Dependencies/FREETYPE Testapp/Dependencies/FMOD)
link_directories(Testapp/Dependencies/)
add_executable(Testapp Testapp/Testapp.cpp)
find_package(GLEW REQUIRED)
find_package(FREETYPE REQUIRED)
find_package(GLFW REQUIRED)
find_package(FMOD REQUIRED)
target_link_libraries(Testapp glfw3 glew32 fmod_vc )



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
