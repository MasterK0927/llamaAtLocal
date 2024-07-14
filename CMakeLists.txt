cmake_minimum_required(VERSION 3.10)
project(MyLlamaApp)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)

add_executable(MyLlamaApp
    main.cpp
    llama.cpp
    llama_server.cpp
    web_search.cpp
    web_search.h
)

target_link_libraries(MyLlamaApp CURL::libcurl)