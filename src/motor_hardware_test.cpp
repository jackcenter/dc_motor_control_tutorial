#include <chrono>
#include <thread>
#include <stdio.h>    // printf()
#include <pigpio.h>
#include "motor_interface.h"

// TODO: finish writing tests

void print_header();
motor::Motor setup_motor();


int main()
{
  print_header();

  motor::Motor test_motor = setup_motor();
  test_motor.print_pins();

  printf("Starting tests.\n");
  printf("\n");

  printf("-- Test 1: activate motor --\n");
  test_motor.init();
  test_motor.activate();
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 2: driving motor counterclockwise --\n");
  test_motor.drive(20);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  test_motor.drive(40);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 3: stopping motor --\n");
  test_motor.stop();
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 4: driving motor clockwise --\n");
  test_motor.drive(-20);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  test_motor.drive(-40);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 5: braking motor --\n");
  test_motor.brake();
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 6: passing invalid commands --\n");
  test_motor.drive(120);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  test_motor.drive(-120);
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  printf("-- Test 7: deactivate motor --\n");
  test_motor.deactivate();
  printf("\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  gpioTerminate();

  return 0;
}


void print_header()
{
  printf("\n");
  printf("==============================================\n");
  printf("==============================================\n");
  printf("            Functional Motor Test:            \n");
  printf("              RPi 4 to TB6612FNG              \n");
  printf("                                              \n");
  printf("             Author: Jack Center              \n");
  printf("==============================================\n");
  printf("==============================================\n");
  printf("\n");
}


motor::Motor setup_motor()
{
  int const in1_pin = 27;
  int const in2_pin = 22;
  int const pwm_pin = 18;
  int const stby_pin = 17;
  int const diretion = 1;

  motor::Motor motor{ in1_pin, in2_pin, pwm_pin, stby_pin, diretion };

  return motor;
}
