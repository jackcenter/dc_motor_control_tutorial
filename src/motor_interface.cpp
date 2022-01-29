#include <stdio.h>      // printf()
#include <stdlib.h>     // abs()
#include <pigpio.h>
#include "motor_interface.h"


namespace  motor
{

void Motor::init()
{
  printf("Initializing motor.\n");

  // General setup
  gpioInitialise();

  // GPIO pin setup
  gpioSetMode(in1_pin_, PI_OUTPUT);
  gpioSetMode(in2_pin_, PI_OUTPUT);
  gpioPWM(pwm_pin_, pwm_duty_cycle_);
  gpioSetMode(stby_pin_, PI_OUTPUT);

  gpioSetPWMfrequency(pwm_pin_, pwm_period_);
  gpioSetPWMrange(pwm_pin_, 100);

  status_ = initialized;
}

void Motor::activate()
{
  printf("Activating motor.\n");

  gpioPWM(pwm_pin_, 0);
  gpioWrite(in1_pin_, 0);
  gpioWrite(in2_pin_, 0);
  gpioWrite(stby_pin_, 1);

  status_ = active;
}

void Motor::deactivate()
{
  printf("Deactivating motor.\n");

  gpioPWM(pwm_pin_, 0);
  gpioWrite(in1_pin_, 0);
  gpioWrite(in2_pin_, 0);
  gpioWrite(stby_pin_, 0);
  
  status_ = initialized;
}

void Motor::drive(int const cmd)
{
  printf("Drive command received: %i.\n", cmd);

  if (status_ != active)
  {
    printf("[Error]: drive command received while motor was not in an active state\n");
    return;
  }

  if (!check_valid_drive_command(cmd))
  {
    printf("[Error]: invalid drive command passed to the motor. Drive command ignored.\n");
    return;
  }

  bool success = true;
  int direction = 0;

  if (cmd * direction_ < 0)
  {
    success = drive_clockwise(abs(cmd));
    direction = -1;
  } else if (0 < cmd * direction_)
  {
    success = drive_counterclockwise(abs(cmd));
    direction = 1;
  } else
  {
    stop();
    direction = 0;
  }

  if (success)
  {
    pwm_duty_cycle_ = abs(cmd);
    current_direction_ = direction;
  } else
  {
    printf("[Error]: drive command was not accepted.\n");
  }
}

void Motor::stop()
{
  printf("Stopping.\n");

  pwm_duty_cycle_ = 100;

  gpioWrite(in1_pin_, 0);
  gpioWrite(in2_pin_, 0);
  gpioPWM(pwm_pin_, pwm_duty_cycle_);
}

void Motor::brake()
{
  printf("Braking.\n");

  pwm_duty_cycle_ = 0;

  gpioWrite(in1_pin_, 1);
  gpioWrite(in2_pin_, 1);
  gpioPWM(pwm_pin_, pwm_duty_cycle_);
}

bool Motor::drive_counterclockwise(int duty_cycle)
{
  printf("Driving counterclockwise at %i percent.\n", duty_cycle);

  if (!check_valid_duty_cycle(duty_cycle))
  {
    printf("[Error]: invalid duty cycle passed to the motor. Drive command ignored.\n");
    return false;
  }
  pwm_duty_cycle_ = duty_cycle;

  gpioWrite(in1_pin_, 0);
  gpioWrite(in2_pin_, 1);
  gpioPWM(pwm_pin_, pwm_duty_cycle_);

  return true;
}

bool Motor::drive_clockwise(int duty_cycle)
{
  printf("Driving clockwise at %i percent.\n", duty_cycle);

  if (!check_valid_duty_cycle(duty_cycle))
  {
    printf("[Error]: invalid duty cycle passed to the motor. Drive command ignored.\n");
    return false;
  }
  pwm_duty_cycle_ = duty_cycle;

  gpioWrite(in1_pin_, 1);
  gpioWrite(in2_pin_, 0);
  gpioPWM(pwm_pin_, pwm_duty_cycle_);

  return true;
}

bool Motor::check_valid_drive_command(int cmd) const
{
  bool result = true;

  if (cmd < -100 || 100 < cmd)
  {
    printf("[Warning]: a drive command of %i is invalid.\n", cmd);
    result = false;
  }

  return result;
}

bool Motor::check_valid_duty_cycle(int duty_cycle) const
{
  bool result = true;

  if (duty_cycle < 0 || 100 < duty_cycle)
  {
    printf("[Warning]: a drive command of %i is invalid.\n", duty_cycle);
    result = false;
  }

  return result;
}

int Motor::get_duty_cycle() const
{
  return pwm_duty_cycle_;
}

int Motor::get_current_direction() const
{
  return current_direction_; 
}

void Motor::print_pins()
{
  printf("\tMotor Pins:\t");
  printf("\n");
  printf("\t In 1 Pin:\t%i\n", in1_pin_);
  printf("\t In 2 Pin:\t%i\n", in2_pin_);
  printf("\t PWM Pin:\t%i\n", pwm_pin_);
  printf("\t In 1 Pin:\t%i\n", stby_pin_);
  printf("\n");
}

} // namespace motor
