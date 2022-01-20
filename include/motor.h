#include <stdio.h>    // printf()
#include <stdlib.h>   // abs()

// TODO: finish writing api
// TODO: add in gpio functionality
namespace motor
{

class Motor
{
  
  public:
    Motor ();
    Motor (int in1_pin, int in2_pin, int pwm_pin, int stby_pin, int direction):
          in1_pin_(in1_pin), 
          in2_pin_(in2_pin), 
          pwm_pin_(pwm_pin), 
          stby_pin_(stby_pin),
          direction_(direction)
          {} 

    void init();
    void activate();
    void deactivate();
    void drive(int const cmd);
    void stop();
    void brake();

    int get_duty_cycle() const;
    int get_current_direction() const;

    void print_pins();

  private:
    int const in1_pin_;
    int const in2_pin_;
    int const pwm_pin_;
    int const stby_pin_;
    int const direction_;

    int pwm_duty_cycle_ = 0;
    int pwm_period_ = 1000;
    int current_direction_ = 0;

    bool drive_counterclockwise(int duty_cycle);
    bool drive_clockwise(int duty_cycle);
    bool check_valid_drive_command(int const cmd) const;
    bool check_valid_duty_cycle(int const duty_cycle) const;

    enum Status
    {
      uninitialized,
      initialized,
      active
    };

    Status status_ = { uninitialized };

};

void Motor::init()
{
  status_ = initialized;
}

void Motor::activate()
{
  status_ = active;
}

void Motor::deactivate()
{
  status_ = initialized;
}

void Motor::drive(int const cmd)
{
  printf("Drive command received: %i.", cmd);

  if (status_ != active)
  {
    printf("[Error]: drive command received while motor was not in an active state");
    return;
  }

  if (!check_valid_drive_command(cmd))
  {
    printf("[Error]: invalid drive command passed to the motor. Drive command ignored.");
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
    printf("[Error]: drive command was not accepted.");
  }
}

void Motor::stop()
{

}

void Motor::brake()
{

}

bool Motor::drive_counterclockwise(int duty_cycle)
{
  return true;
}

bool Motor::drive_clockwise(int duty_cycle)
{
  return true;
}

bool Motor::check_valid_drive_command(int cmd) const
{
  bool result = true;

  if (cmd < -100 || 100 < cmd)
  {
    printf("[Warning]: a drive command of %i is invalid.", cmd);
    result = false;
  }

  return result;
}

bool Motor::check_valid_duty_cycle(int duty_cycle) const
{
  bool result = true;

  if (duty_cycle < 0 || 100 < duty_cycle)
  {
    printf("[Warning]: a drive command of %i is invalid.", duty_cycle);
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
  // TODO: change this to printf
  printf("\n");
  printf("\tMotor Pins:\t");
  printf("\n");
  printf("\t In 1 Pin:\t%i", in1_pin_);
  printf("\t In 2 Pin:\t%i", in2_pin_);
  printf("\t PWM Pin:\t%i", pwm_pin_);
  printf("\t In 1 Pin:\t%i", stby_pin_);
  printf("\n");
}

} // namespace motor
