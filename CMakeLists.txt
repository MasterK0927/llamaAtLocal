cmake_minimum_required(VERSION 3.10)
project(LlamaChat)

set(CMAKE_CXX_STANDARD 17)

# Define the path to llama.cpp
set(LLAMA_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/llama.cpp)

# Include llama.cpp source files
file(GLOB LLAMA_CPP_SOURCES 
    ${LLAMA_CPP_DIR}/*.cpp
    ${LLAMA_CPP_DIR}/*.h
    ${LLAMA_CPP_DIR}/examples/common.cpp
)

# Add our source files
add_executable(llamachat 
    src/main.cpp
    src/TerminalUI.cpp
    src/LlamaChat.cpp
    ${LLAMA_CPP_SOURCES}
)

# Include directories
target_include_directories(llamachat PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LLAMA_CPP_DIR}
)

# Add compile definitions to help find llama.h
target_compile_definitions(llamachat PRIVATE LLAMA_CPP_DIR="${LLAMA_CPP_DIR}")

# Link against required libraries
target_link_libraries(llamachat PRIVATE ncurses pthread)

# Install targets
install(TARGETS llamachat DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/models/phi.gguf DESTINATION share/llamachat)

# Compile definitions for llama.cpp
target_compile_definitions(llamachat PRIVATE GGML_USE_K_QUANTS)

# Print include directories for debugging
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "Include dir='${dir}'")
endforeach()