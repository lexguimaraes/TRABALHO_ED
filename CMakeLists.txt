cmake_minimum_required(VERSION 3.22.5)
project(TRABALHO_ED C)

set(CMAKE_C_STANDARD 99)


add_executable (DESC_TEST main.c ARVBP.c TIM.c extras.c TLSEIM.c DESC.c)
target_link_libraries(DESC_TEST m)
