cmake_minimum_required(VERSION 3.5)
project(LastnameFirstname_PA1 CXX)

add_executable(L1simulate L1simulate.cpp)
target_compile_options(L1simulate PUBLIC -Wall -Werror -Wextra -g)
