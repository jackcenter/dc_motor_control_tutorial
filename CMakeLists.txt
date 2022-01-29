cmake_minimum_required(VERSION 3.0.2)
project(DC_Motor_Control_Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED) 
find_package(pigpio REQUIRED)

include_directories(
	${pigpio_INCLUDE_DIR}
	include)

add_library(motor_lib src/motor_interface.cpp include/motor_interface.h)
add_executable(motor_hardware_test src/motor_hardware_test.cpp)
target_link_libraries(motor_hardware_test 
	${pigpio_LIBRARY}
	pthread
	motor_lib
	)

add_library(serial_lib src/serial_interface.cpp include/serial_interface.h)
add_executable(encoder_hardware_test src/encoder_hardware_test.cpp)
target_link_libraries(encoder_hardware_test 
	${Boost_LIBRARIES}	
	serial_lib
	)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests test/motor_tests.cpp)
target_link_libraries(runTests 
	${GTEST_LIBRARIES} 
	${pigpio_LIBRARY}
	pthread
	motor_lib
	)
