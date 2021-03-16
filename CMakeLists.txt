cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 17)

# project NAME
project(TestBER)

# poco library
add_subdirectory(poco)
# link together
file(GET_RUNTIME_DEPENDENCIES)

#set(SRC_DIR src/)
#list(CPP IO.cpp BER.cpp send_and_receiveData.cpp)

# add the executable
add_executable(server src/Server.cpp src/IO.cpp src/BER.cpp src/send_and_receiveData.cpp)
add_executable(client src/Client.cpp src/IO.cpp src/BER.cpp src/send_and_receiveData.cpp)

#add_executable(server ${SRC_DIR}Server.cpp ${SRC_DIR}${CPP})
#add_executable(client ${SRC_DIR}Client.cpp ${SRC_DIR}${CPP})

# include shared library
find_package(Poco REQUIRED Net Util)
target_link_libraries(server PUBLIC Poco::Net Poco::Util)
target_link_libraries(client PUBLIC Poco::Net Poco::Util)
# poco in github https://github.com/pocoproject/poco

# include headers
include_directories(include/)