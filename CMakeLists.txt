# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.8.9)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(Graph_cut)

# Find ITK.
find_package(ITK REQUIRED)
find_package(VTK REQUIRED)

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

add_executable(Graph_cut Graph_cut.cxx )

target_link_libraries(Graph_cut ${ITK_LIBRARIES} ${VTK_LIBRARIES})
