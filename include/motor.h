#include <iostream>

// TODO: finish writing api

class Motor
{
  
  public:
    Motor ();
    Motor (int in1_pin, int in2_pin, int pwm_pin, int stby_pin, int direction):
          in1_pin(in1_pin), 
          in2_pin(in2_pin), 
          pwm_pin(pwm_pin), 
          stby_pin(stby_pin),
          direction(direction)
          {} 

    void print_pins();

  private:
    int in1_pin;
    int in2_pin;
    int pwm_pin;
    int stby_pin;
    int direction;

    int pwm_duty_cycle = 0;
    int pwm_period = 1000;

};

void Motor::print_pins()
{
  std::cout << std::endl;
  std::cout << "\tMotor Pins:\t" << std::endl;
  std::cout << std::endl;
  std::cout << "\t In 1 Pin:\t" << in1_pin << std::endl;
  std::cout << "\t In 2 Pin:\t" << in2_pin << std::endl;
  std::cout << "\t PWM Pin:\t" << pwm_pin << std::endl;
  std::cout << "\t In 1 Pin:\t" << stby_pin << std::endl;
  std::cout << std::endl;
}
