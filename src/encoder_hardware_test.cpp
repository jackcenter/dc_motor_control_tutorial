#include <chrono>
#include <thread>
#include <stdio.h>    // printf()
#include <motor.h>

void print_header();
motor::Motor setup_motor();


int main()
{
  print_header();

  motor::Motor test_motor = setup_motor();
  test_motor.print_pins();
  
}


void print_header()
{
  printf("\n");
  printf("==============================================\n");
  printf("==============================================\n");
  printf("           Functional Encoder Test:           \n");
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