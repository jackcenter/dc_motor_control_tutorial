#include <chrono>
#include <iostream>
#include <thread>
#include <stdio.h>    // printf()
#include <string>
#include <boost/asio.hpp>
#include "encoder_interface.h"
#include "serial_interface.h"

using namespace std::chrono_literals;

void print_header();


int main()
{
  print_header();

  int cpr = 8400;
  int rate = 500;

  encoder::Encoder encoder{cpr, rate};

  serial::Serial serial_port{"/dev/ttyACM0"};
  serial_port.open();

  std::string cmd = "a";
  auto time_loop = 2ms;
  auto time_start = std::chrono::system_clock::now();

  std::string response_string1;
  std::string response_string2;

  int count = 0;

  while (true)
  {
    // TODO: make this asynch


    if (time_loop < std::chrono::system_clock::now() - time_start)
    {
      serial_port.write(cmd);
      response_string1 = serial_port.read_line();   // enc value
      response_string2 = serial_port.read_line();   // time

      long enc_val = std::stol(response_string1);

      encoder.update(enc_val);

      ++count;
      if (count % 50 == 0)
      {
        encoder.print_values();
      }


      // std::cout << response_string1 << ", " << response_string2 << std::endl;

      time_start += time_loop;
    }
  }

  serial_port.close();

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
