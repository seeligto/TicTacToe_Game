cmake_minimum_required(VERSION 3.27)
project(HFU_TicTacToe_tt)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HFU_TicTacToe_tt
        ../TicTacToe_Game/main.cpp
        ../TicTacToe_Game/spielfeld.cpp
        ../TicTacToe_Game/spielfeld.hh)
