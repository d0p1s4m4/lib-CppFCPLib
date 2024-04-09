/*
 * Copyright (c) 2024 d0p1 <contact@d0p1.eu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of mosquitto nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef FCP_CLIENT_HPP_
#define FCP_CLIENT_HPP_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fcp++/node.hpp>
#include <fcp++/protocol/request.hpp>
#include <fcp++/ssk/keypair.hpp>
#include <string>
#include <vector>


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

  std::vector<Node> ListPeer(Node node);
  std::vector<Node> ListPeers();

  Node GetNode();

  ssk::KeyPair GenerateSSK();

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
  this->mSocket.send(boost::asio::buffer(data.ToRequest().ToString()));
}

}

#endif // !FCP_CLIENT_HPP_