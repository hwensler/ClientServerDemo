cmake_minimum_required(VERSION 3.6)

project(ClientServerGuessingGame)


set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES "Client.cpp" "Server.cpp")


add_executable(ClientServerGuessingGame ${SOURCE_FILES})


set_target_properties(ClientServerGuessingGame PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(ClientServerGuessingGame pthread)