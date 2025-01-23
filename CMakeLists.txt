cmake_minimum_required(VERSION 3.22.1)
project(TRABALHO_ED C)

set(CMAKE_C_STANDARD 99)

add_executable(TRABALHO_ED main.c bibliotecas/ARVBP.c bibliotecas/TIM.c bibliotecas/extras.c bibliotecas/TLSEIM.c)
add_executable (DESC_TEST main.c ARVBP.c TIM.c extras.c TLSEIM.c DESC.c)
add_executable(TIM bibliotecas/TIM.c)
add_executable(ARVBP bibliotecas/ARVBP.c bibliotecas/TIM.c)