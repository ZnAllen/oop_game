cmake_minimum_required(VERSION 3.10)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ENET_DIR ${CMAKE_SOURCE_DIR}/../../lib/enet)
include_directories(${ENET_DIR}/include)
link_directories(${ENET_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/../UI/include)
link_directories(${CMAKE_SOURCE_DIR}/../UI/lib)

set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/../../lib/raylib)
include_directories(${RAYLIB_DIR}/include)
link_directories(${RAYLIB_DIR}/lib)


add_executable(Client client.cpp ../UI/GameMap.cpp)
target_link_libraries(Client PRIVATE
    ${ENET_DIR}/lib/libenet.a  # 使用完整路徑（如果自動搜尋不成功）
    raylib
    ws2_32                    # Windows 套接字庫
    winmm                     # Windows 多媒體庫（ENet 需要的）
)
target_link_libraries(Client PRIVATE ${CMAKE_SOURCE_DIR}/../UI/lib)
target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/../UI)