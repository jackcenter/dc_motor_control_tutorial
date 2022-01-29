#include "serial.h"


namespace serial
{

std::string Serial::read_line()
{
  std::string response_string;

  boost::asio::read_until(port_, response_buffer_, '\n');
  std::getline(response_stream_, response_string);
  response_string.pop_back();

  return response_string;
}

void Serial::write (std::string const & cmd)
{
  boost::asio::write(port_, boost::asio::buffer(cmd));
}

} // namespace serial
