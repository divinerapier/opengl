set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(use-index-buffer-to-draw-triangles src/main.cpp)
target_link_libraries(use-index-buffer-to-draw-triangles glfw GLEW GL)