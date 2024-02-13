cmake_minimum_required(VERSION 3.1)
project(AVR_tlm_sim)

# Add a CMake option to enable/disable debug mode
option(DEBUG "Enable Debug Mode" OFF)

# Check if DEBUG is enabled and set the appropriate compiler flag
if(DEBUG)
    message(STATUS "Debug mode enabled")
    add_compile_definitions(DEBUG) # Define DEBUG macro
else()
    message(STATUS "Debug mode disabled")
endif()

#add_definitions(-DLOG_LEVEL_INFO)
#add_definitions(-DLOG_LEVEL_ERROR)
#add_definitions(-DLOG_LEVEL_ALL)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3 -g -Wall -Wextra -Wunused-function -Wpedantic)

include_directories(./include/)
file(GLOB SRC "./src/*.cpp")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17) # Assuming you want to use C++17

# Include directories for SystemC
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/systemc-2.3.3/src)

# Adding the thirdparty lib directory to the link directories
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/systemc-2.3.3/build/src/sysc/.libs)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/systemc-2.3.3/build/src/tlm_core/.libs)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/systemc-2.3.3/build/src/tlm_utils/.libs)

# Define your executable or library target
add_executable(AVR_tlm_sim ${SRC})

# Linking SystemC library
target_link_libraries(AVR_tlm_sim sysc tlm_core tlm_utils)

