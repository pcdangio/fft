cmake_minimum_required(VERSION 3.5)

# Define project.
project(fft
        VERSION 1.0.0)

# Set up includes.
include_directories(include)

# Build library.
add_library(pcd-fft STATIC src/fft.cpp)

# Install library.
install(TARGETS pcd-fft
        ARCHIVE DESTINATION lib)
# Install public headers.
install(DIRECTORY include/
        DESTINATION include)