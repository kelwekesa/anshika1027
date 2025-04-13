cmake_minimum_required(VERSION 3.5)
project(LastnameFirstname_PA1 CXX)
include_directories(include)

file(GLOB SRC CONFIGURE_DEPENDS "include/*.h" "src/*.cpp")
add_executable(L1simulate ${SRC})
target_compile_options(L1simulate PUBLIC -Wall -Werror -Wextra -g)
