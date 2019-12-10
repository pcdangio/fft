cmake_minimum_required(VERSION 3.5)

# Define project.
project(fft
        VERSION 1.0.0)

# Set up includes.
include_directories(include)

# Build library.
add_library(pcd-fft STATIC src/fft.cpp)

# Build tests.
file(GLOB test_cpps test/*.cpp)
foreach(test_cpp ${test_cpps})
  get_filename_component(target_name ${test_cpp} NAME_WE)  
  add_executable(${target_name} ${test_cpp})
  target_link_libraries(${target_name} pcd-fft)
endforeach()

# Install library.
install(TARGETS pcd-fft
        ARCHIVE DESTINATION lib)
# Install public headers.
install(DIRECTORY include/
        DESTINATION include)