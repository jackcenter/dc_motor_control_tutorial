#include <chrono>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <stdio.h>    // printf()
#include <motor.h>

void print_header();
motor::Motor setup_motor();


int main()
{
  print_header();

  motor::Motor test_motor = setup_motor();
  test_motor.print_pins();

  boost::asio::io_service io;
  boost::asio::serial_port port(io);
  port.open("/dev/ttyACM0");
  port.set_option(boost::asio::serial_port_base::baud_rate(115200));

  // This works by blocking which isn't great. It sends a char then waits to get a char
  // cmd a = send enc value and time
  char cmd[1] = {'a'};
  boost::asio::write(port, boost::asio::buffer(cmd, 1));
  
  // buffers
  boost::asio::streambuf response;
  std::istream response_stream(&response);
  std::string response_string;

  // enc value
  boost::asio::read_until(port, response, '\n');
  response_stream >> response_string;
  std::cout << response_string << std::endl;

  // time
  boost::asio::read_until(port, response, '\n');
  response_stream >> response_string;
  std::cout << response_string << std::endl;

  port.close();
  
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