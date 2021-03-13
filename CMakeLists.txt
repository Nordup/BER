cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 14)

# project NAME
project(TestBER)

# add the executable
add_executable(server src/Server.cpp)
add_executable(client src/Client.cpp)

# include shared library
find_package(Poco REQUIRED Net Util)
target_link_libraries(server PUBLIC Poco::Net Poco::Util)
target_link_libraries(client PUBLIC Poco::Net Poco::Util)

# include header
include_directories(include/)