include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(Mixer_demo mixer_demo.cpp )

add_executable(Simple_size_demo size_demo.cpp)
target_link_libraries(Simple_size_demo bloom)

