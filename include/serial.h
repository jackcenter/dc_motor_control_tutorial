#pragma once

#include <iostream>
#include <boost/asio.hpp>


namespace serial
{

class Serial
{
  public:
    Serial (std::string address)
      : io_service_ {}
      , port_ {io_service_}
      , response_buffer_ {}
      , response_stream_ {&response_buffer_}
      , address_ {address}
    {
    }

    void open() { port_.open(address_); };
    void close() { port_.close(); };

    std::string read_line();
    void write(std::string const & cmd);

  private:
    boost::asio::io_service io_service_;
    boost::asio::serial_port port_;
    boost::asio::streambuf response_buffer_;
    std::istream response_stream_;
    std::string address_;

};

} // namespace serial

