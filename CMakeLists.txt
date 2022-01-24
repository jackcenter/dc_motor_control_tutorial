cmake_minimum_required(VERSION 3.0.2)
project(DC_Motor_Control_Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

find_package(pigpio REQUIRED)
include_directories(${pigpio_INCLUDE_DIR})

add_executable(motor_hardware_test src/motor_hardware_test.cpp)
target_link_libraries(motor_hardware_test 
	${pigpio_LIBRARY}
	)

add_executable(encoder_hardware_test src/encoder_hardware_test.cpp)
target_link_libraries(encoder_hardware_test 
	${pigpio_LIBRARY}
	)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests test/motor_tests.cpp)
target_link_libraries(runTests 
	${GTEST_LIBRARIES} 
	pthread
	${pigpio_LIBRARY}
	)
