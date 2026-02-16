cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(podere_pannocchia_monopoly) 

include_directories(include) 

add_library(Board
	include/Board.h
	src/Board.cpp
)

add_library(Cell
	include/Cell.h
	src/Cell.cpp
)

add_library(Player
	include/Player.h
	src/Player.cpp
)

add_executable(Game src/Game.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

target_link_libraries(Game Board Cell Player)

install(TARGETS Game DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
