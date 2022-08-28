# cmake_minimum_required(VERSION 3.15)
# project(MyProject)

# set(CMAKE_CXX_STANDARD 17)

# # Set the build output location to a common directory
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# add_subdirectory(OpenXLSX)
# # set(JSON_Install OFF CACHE INTERNAL "")
# add_subdirectory(nlohmann_json)

# add_executable(MyProject main.cpp)
# target_link_libraries(MyProject PRIVATE nlohmann_json::nlohmann_json)
# target_link_libraries(MyProject OpenXLSX::OpenXLSX)
#

cmake_minimum_required(VERSION 3.1)
project(ExampleProject)

# If you only include this third party in PRIVATE source files, you do not need to install it
# when your main project gets installed.
set(JSON_Install OFF CACHE INTERNAL "")

# Set the build output location to a common directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

add_subdirectory(nlohmann_json)
add_subdirectory(OpenXLSX)

add_executable(ExampleProject main.cpp Table.cpp)
target_link_directories(ExampleProject PRIVATE ${PROJECT_BINARY_DIR}/headers)
target_link_libraries(ExampleProject OpenXLSX::OpenXLSX nlohmann_json::nlohmann_json)
