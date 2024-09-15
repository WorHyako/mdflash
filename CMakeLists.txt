cmake_minimum_required(VERSION 3.10)
project(mdflash VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(mdflash
    main.cpp
    markdown_processor.cpp
)

target_link_libraries(mdflash PRIVATE Qt5::Widgets)

install(TARGETS mdflash DESTINATION bin)

