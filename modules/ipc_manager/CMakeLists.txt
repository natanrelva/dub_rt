cmake_minimum_required(VERSION 3.10)
project(IPCManagerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona o executável principal da aplicação
add_executable(IPCApp Main.cpp)

# Linkar pthread se usar threads
target_link_libraries(IPCApp pthread)
