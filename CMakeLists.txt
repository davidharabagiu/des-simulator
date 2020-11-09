cmake_minimum_required(VERSION 3.17)
project(des_simulator)

set(CMAKE_CXX_STANDARD 14)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

add_executable(des_simulator main.cpp dessimulator.cpp dessimulator.h simulatorframe.cpp simulatorframe.h interactivedescipher.cpp interactivedescipher.h)
target_link_libraries(des_simulator ${wxWidgets_LIBRARIES})
