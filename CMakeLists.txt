cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 17)

# project NAME
project(TestBER)

# poco library
add_subdirectory(poco)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_BINARY_DIR}/poco/Poco")
include(PocoConfig)

# add the executable
add_executable(server src/Server.cpp src/IO.cpp src/BER.cpp src/send_and_receiveData.cpp)
add_executable(client src/Client.cpp src/IO.cpp src/BER.cpp src/send_and_receiveData.cpp)

# include shared library
#find_package(Poco REQUIRED Net)
target_link_libraries(server PUBLIC Poco::Net)
target_link_libraries(client PUBLIC Poco::Net)
# poco in github https://github.com/pocoproject/poco

# include headers
include_directories(include/)