# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.8.9)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(3DConfidenceConnected)

# Find ITK.
find_package(ITK REQUIRED)
find_package(VTK REQUIRED)

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

add_executable(3DConfidenceConnected 3DConfidenceConnected.cxx )

target_link_libraries(3DConfidenceConnected ${ITK_LIBRARIES} ${VTK_LIBRARIES})
