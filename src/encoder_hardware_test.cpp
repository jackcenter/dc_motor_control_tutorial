#include <chrono>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <stdio.h>    // printf()
#include <motor.h>

using namespace std::chrono_literals;

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
  
  // buffers
  boost::asio::streambuf response;
  std::istream response_stream(&response);
  std::string response_string1;
  std::string response_string2;
  char cmd[1] = {'a'};
  auto time_loop = 2ms;
  auto time_start = std::chrono::system_clock::now();

  while (true)
  {
    // TODO: make this asynch
    // TODO: make functions for this

    if (time_loop < std::chrono::system_clock::now() - time_start)
    { 
      boost::asio::write(port, boost::asio::buffer(cmd));

      // enc value
      boost::asio::read_until(port, response, '\n');
      std::getline(response_stream, response_string1);
      response_string1.pop_back();

      // time
      boost::asio::read_until(port, response, '\n');
      std::getline(response_stream, response_string2);
      response_string2.pop_back();

      std::cout << response_string1 << ", " << response_string2 << std::endl;

      time_start += time_loop;
    }
  }

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