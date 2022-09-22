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
project(AuditTables)

# If you only include this third party in PRIVATE source files, you do not need to install it
# when your main project gets installed.
set(JSON_Install OFF CACHE INTERNAL "")

# Set the build output location to a common directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

add_subdirectory(nlohmann_json)
add_subdirectory(OpenXLSX)
add_subdirectory(xls2xlsx_Master)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(${PROJECT_NAME} main.cpp Table.cpp)
target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR}/headers)
target_link_libraries(${PROJECT_NAME} OpenXLSX::OpenXLSX nlohmann_json::nlohmann_json)
