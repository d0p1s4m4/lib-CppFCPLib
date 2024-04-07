#ifndef FCP_CLIENT_HPP_
#define FCP_CLIENT_HPP_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fcp++/node.hpp>
#include <fcp++/protocol/request.hpp>
#include <string>

namespace fcp {

const std::string default_host = "127.0.0.1";
const unsigned short default_port = 9481;

class Client
{
public:
  Client(const std::string& name);
  ~Client();

  int Connect(const std::string& host = default_host,
              unsigned short port = default_port);
  int Connect(const boost::asio::ip::tcp::endpoint& endpoint);
  void Disconnect();

  template<class Data>
  void Send(Data data);
  Node GetNode();

  void Shutdown();

private:
  boost::asio::io_service mIOService;
  boost::asio::ip::tcp::socket mSocket;
  const std::string& mAppName;
};

template<class Data>
void
Client::Send(Data data)
{
  this->mSocket.send(boost::asio::buffer(data.ToReq().ToString()));
}

}

#endif // !FCP_CLIENT_HPP_