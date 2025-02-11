cmake_minimum_required(VERSION 3.15)
project(ORM_Demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(orm_demo
    main.cpp
    User.hpp
    Database.hpp
    ORM.hpp
)

target_include_directories(orm_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 