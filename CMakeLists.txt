cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(Ranking VERSION 1.0 DESCRIPTION "Input data and rank!")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
file(GLOB SOURCES "src/*.cc" "src/*.c")
add_executable(Ranking ${SOURCES})
target_link_libraries(Ranking SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
target_include_directories(Ranking PRIVATE "include")
