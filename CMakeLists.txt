cmake_minimum_required(VERSION 3.10)

project(mdflash VERSION 0.6.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5
        COMPONENTS Widgets
        REQUIRED)

add_executable(mdflash)

target_sources(mdflash
        PRIVATE
        main.cpp
        markdown_processor.cpp)

target_link_libraries(mdflash
        PRIVATE Qt5::Widgets)

install(TARGETS mdflash
        DESTINATION bin)
