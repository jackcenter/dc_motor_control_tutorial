#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <stdio.h>    // printf()
#include <string>
#include <boost/asio.hpp>
#include <pigpio.h>
#include "encoder_interface.h"
#include "motor_interface.h"
#include "serial_interface.h"

using namespace std::chrono_literals;

void print_header();
encoder::Encoder setup_encoder();
long read_encoder_value(serial::Serial& serial_port);
motor::Motor setup_motor();
void run_peaks_test(encoder::Encoder& encoder, motor::Motor& motor, serial::Serial& serial_port, int const test_number);
void run_drive_test(int const speed, encoder::Encoder& encoder, motor::Motor& motor, serial::Serial& serial_port, int const test_number);
int calculate_peaks_expected(double const test_duration, double const signal_freq);
int test_signal(double const a, double const f, double const s, double const t);
bool check_direction_change(long const val_k0, long const val_k1);


int main()
{
  print_header();

  encoder::Encoder encoder = setup_encoder();

  motor::Motor motor = setup_motor();
  motor.init();
  motor.activate();

  serial::Serial serial_port{"/dev/ttyACM0"};
  serial_port.open();

  run_peaks_test(encoder, motor, serial_port, 1);
  printf("\n");
  std::this_thread::sleep_for(1s);

  run_drive_test(40, encoder, motor, serial_port, 2);
  printf("\n");
  std::this_thread::sleep_for(1s);

  run_drive_test(-40, encoder, motor, serial_port, 3);
  printf("\n");
  std::this_thread::sleep_for(1s);

  motor.deactivate();
  serial_port.close();
  std::this_thread::sleep_for(1s);

  gpioTerminate();

  printf("Tests complete.\n");
  printf("\n");

  return 0;
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

void run_peaks_test(encoder::Encoder& encoder, motor::Motor& motor, serial::Serial& serial_port, int const test_number)
{
  printf("-- Test %i: count encoder peaks --\n", test_number);
  auto time_loop = 2ms;
  auto time_test = 2250ms;

  double const amp = 40;
  double const freq = 2;
  double const shift = 0;

  int peaks_expected = calculate_peaks_expected(time_test.count() / 1000, freq);

  int count = 0;
  int dir_current = 1;
  int peaks = 0;

  std::this_thread::sleep_for(1s);

  auto time_start = std::chrono::system_clock::now();
  auto time_loop_start = time_start;

  while (std::chrono::system_clock::now() - time_start < time_test)
  {
    if (time_loop < std::chrono::system_clock::now() - time_loop_start)
    {
      auto time_test_elapsed = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start);
      int signal_value = test_signal(amp, freq, shift, time_test_elapsed.count());
      motor.drive(signal_value);

      long enc_val = read_encoder_value(serial_port);
      encoder.update(enc_val);

      long const dir_encoder = encoder.get_direction();
      if (check_direction_change(dir_current, dir_encoder))
      {
        ++peaks;
        dir_current = dir_encoder;
      }

      time_loop_start += time_loop;
    }
  }

  motor.stop();

  // Result
  if (peaks == peaks_expected)
  {
    printf("\tTest Passed: %i of %i expected peaks measured.\n", peaks, peaks_expected);
  } else
  {
    printf("\tTest Failed: %i of %i expected peaks measured.\n", peaks, peaks_expected);
  }

  motor.brake();
}

void run_drive_test(int const speed, encoder::Encoder& encoder, motor::Motor& motor, serial::Serial& serial_port, int const test_number)
{
  int const direction = speed / abs(speed);

  std::string title;
  if (direction == 1)
  {
    title = "drive forward";
  } else
  {
    title = "drive backward";
  }

  std::cout << "-- Test " << test_number << ": " << title << " --\n";
  auto time_control = 2ms;
  auto time_measurement = 50ms;
  auto time_test = 1000ms;

  motor.drive(speed);
  std::this_thread::sleep_for(1s);

  int fails_position = 0;
  int fails_velocity = 0;
  int count_measurements = 0;

  auto time_test_start = std::chrono::system_clock::now();
  auto time_control_start = time_test_start;
  auto time_measurement_start = time_test_start;

  while (std::chrono::system_clock::now() - time_test_start < time_test)
  {
    // Control Loop
    if (time_control < std::chrono::system_clock::now() - time_control_start)
    {
      long enc_val = read_encoder_value(serial_port);
      encoder.update(enc_val);

      time_control_start += time_control;
    }

    // Test Loop
    if (time_measurement < std::chrono::system_clock::now() - time_measurement_start)
    {
      if (direction * (encoder.get_current_value() - encoder.get_previous_value()) <= 0)
      {
        ++fails_position;
      }

      if (direction * encoder.get_velocity() <= 0)
      {
        ++fails_velocity;
      }

      ++count_measurements;
      time_measurement_start += time_measurement;
    }
  }

  motor.stop();

  if (fails_position || fails_velocity)
  {
    printf("\tTests Failed. Number of failed measurements:\n");
    printf("\t  Position:\t%i of %i.\n", fails_position, count_measurements);
    printf("\t  Velocity:\t%i of %i.\n", fails_velocity, count_measurements);
  } else
  {
    printf("\tTest Passed:\t%i measurements taken.\n", count_measurements);
  }

  motor.brake();
}

int calculate_peaks_expected(double const test_duration, double const signal_freq)
{
    return (int) 2 * test_duration * signal_freq;
}

encoder::Encoder setup_encoder()
{
  int cpr = 8400;
  int rate = 500;

  encoder::Encoder encoder{cpr, rate};

  serial::Serial serial_port{"/dev/ttyACM0"};
  serial_port.open();
  serial_port.write("a");
  std::string response_string1 = serial_port.read_line();   // enc value
  std::string response_string2 = serial_port.read_line();   // time
  serial_port.close();

  long enc_val = std::stol(response_string1);

  encoder.set_initial_value(enc_val);

  return encoder;
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

int test_signal(double const a, double const f, double const s, double const t)
{
  double val = a*sin(2 * M_PI * f * t - s);
  return (int) std::round(val);
}

long read_encoder_value(serial::Serial& serial_port)
{
  serial_port.write("a");
  std::string response_string1 = serial_port.read_line();   // enc value
  std::string response_string2 = serial_port.read_line();   // time

  return std::stol(response_string1);
}

bool check_direction_change(long const val_k0, long const val_k1)
{
  bool result = false;

  if (val_k1 && val_k1 != val_k0)
  {
    result = true;
  }

  return result;
}
