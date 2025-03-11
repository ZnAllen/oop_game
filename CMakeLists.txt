cmake_minimum_required(VERSION 3.10)
project(oop_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/lib/raylib/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/raylib/lib)

set(ENET_DIR ${CMAKE_SOURCE_DIR}/lib/enet)
include_directories(${ENET_DIR}/include)
link_directories(${ENET_DIR}/lib)

set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/lib/raylib)
include_directories(${RAYLIB_DIR}/include)
link_directories(${RAYLIB_DIR}/lib)

add_executable(oop_game src/main.cpp src/Map.cpp src/Unit.cpp src/pathfinding.cpp src/GameManager.cpp)
target_link_libraries(oop_game
    raylib      # Raylib 庫（會自動找到 libraylib.a）
    winmm       # Windows 多媒體庫
)