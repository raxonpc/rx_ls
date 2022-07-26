cmake_minimum_required(VERSION 3.4)

project(rx_ls LANGUAGES C)

set(FLAGS -Wall -Wextra -pedantic -O2)

add_executable(rx_ls rx_ls.c)
target_compile_options(rx_ls PRIVATE ${FLAGS})

