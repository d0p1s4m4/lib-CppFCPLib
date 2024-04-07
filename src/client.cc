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

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fcp++/client.hpp>
#include <fcp++/protocol/request.hpp>
#include <iostream>

using namespace fcp;
using boost_ipaddr = boost::asio::ip::address;
using boost_tcp = boost::asio::ip::tcp;

Client::Client(const std::string& name)
  : mAppName(name)
  , mSocket(mIOService)
{
}

Client::~Client()
{
  this->Disconnect();
}

int
Client::Connect(const std::string& host, unsigned short port)
{
  return this->Connect(boost_tcp::endpoint(boost_ipaddr::from_string(host), port));
}

int
Client::Connect(const boost_tcp::endpoint& endpoint)
{
  try {
    mSocket.open(endpoint.protocol());

    mSocket.connect(endpoint);

    protocol::request::ClientHello clientHello(this->mAppName);

    this->Send(clientHello);
  } catch (boost::system::system_error& e) {
    std::cerr << e.what() << std::endl;
    return e.code().value();
  }

  return 0;
}

void
Client::Disconnect()
{
  protocol::request::Disconnect disconnect;

  this->Send(disconnect);
}

void
Client::Shutdown()
{
  protocol::request::Shutdown shutdown;

  this->Send(shutdown);
}