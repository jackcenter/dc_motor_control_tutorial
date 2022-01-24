
#include <iostream>
#include <motor.h>

// TODO: finish writing tests

int main()
{
  int const in1_pin = 27;
  int const in2_pin = 22;
  int const pwm_pin = 18;
  int const stby_pin = 17;
  int const diretion = 1;

  motor::Motor test_motor{ in1_pin, in2_pin, pwm_pin, stby_pin, diretion };

  test_motor.print_pins();

  return 0;
}
