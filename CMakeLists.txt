cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 14)

# project NAME
project(PocoTest)

# add the executable
add_executable(server TcpServer.cpp)
add_executable(client TcpClient.cpp)

# include shared library
find_package(Poco REQUIRED Net Util JSON XML)
target_link_libraries(server PUBLIC Poco::Net Poco::Util Poco::JSON Poco::XML)
target_link_libraries(client PUBLIC Poco::Net Poco::Util Poco::JSON Poco::XML)
