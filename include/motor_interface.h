#pragma once


namespace motor
{

class Motor
{
  public:
    Motor (int in1_pin, int in2_pin, int pwm_pin, int stby_pin, int direction)
      : in1_pin_(in1_pin)
      , in2_pin_(in2_pin)
      , pwm_pin_(pwm_pin)
      , stby_pin_(stby_pin)
      , direction_(direction)
    {}

    void set_verbose_output(bool val) { verbose_ = val; };

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

    bool verbose_ = false;
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

} // namespace motor
