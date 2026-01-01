cmake_minimum_required(VERSION 3.30)
project(cpp09)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # don't silently upgrade
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict warnings
#add_compile_options(-Wall -Wextra -Werror)


add_executable(CPP_09_ex00
        ex00/BitcoinExchange.cpp
        ex00/main.cpp)

add_executable(CPP_09_ex01
        ex01/RPN.cpp
        ex01/main.cpp)

add_executable(CPP_09_ex02
        ex02/PmergeMe.cpp
        ex02/main.cpp)

add_executable(test
        test.cpp)