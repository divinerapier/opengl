set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(uniform src/main.cpp)
target_link_libraries(uniform glfw GLEW GL)