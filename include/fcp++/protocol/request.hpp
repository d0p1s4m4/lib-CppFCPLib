#ifndef FCP_REQUEST_HPP_
#define FCP_REQUEST_HPP_

#include <boost/asio/ip/tcp.hpp>
#include <fcp++/node.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace fcp::protocol::request {

class Req
{
public:
  Req(std::string_view name) : mName(name) {};
  virtual ~Req() = default;

  void SetAttribute(std::string aKey, std::string aValue)
  {
    this->mAttributes[aKey] = aValue;
  };

  std::string ToString()
  {
    std::string str(this->mName);
    str += "\n";
    for (auto& it : this->mAttributes) {
      str += it.first;
      str += "=";
      str += it.second;
      str += "\n";
    }
    str += "EndMessage\n";

    return str;
  }

private:
  std::string_view mName;
  std::unordered_map<std::string, std::string> mAttributes;
};

struct ClientHello
{
  std::string Name;
  std::string ExpectedVersion;

  ClientHello(std::string_view name)
    : Name(name)
    , ExpectedVersion("2.0")
  {
  }

  Req ToReq()
  {
    Req req("ClientHello");

    req.SetAttribute("Name", this->Name);
    req.SetAttribute("ExpectedVersion", this->ExpectedVersion);

    return req;
  }
};

struct ListPeer
{
  std::string NodeIdentifier;
  std::optional<bool> WithMetaData;
  std::optional<bool> WithVolatile;

  ListPeer(std::string_view ident)
    : NodeIdentifier(ident)
  {
  }

  Req ToReq()
  {
    Req req("ListPeer");

    req.SetAttribute("NodeIdentifier", this->NodeIdentifier);
    if (this->WithMetaData.has_value()) {
      req.SetAttribute("WithMetaData", to_string(this->WithMetaData.value()));
    }

    if (this->WithVolatile.has_value()) {
      req.SetAttribute("WithVolatile", to_string(this->WithVolatile.value()));
    }

    return req;
  }
};

struct ListPeers
{
  std::optional<std::string> Identifier;
  std::optional<bool> WithMetaData;
  std::optional<bool> WithVolatile;

  Req ToReq()
  {
    Req req("ListPeers");

    if (this->Identifier.has_value()) {
      req.SetAttribute("Identifier", this->Identifier.value());
    }

    if (this->WithMetaData.has_value()) {
      req.SetAttribute("WithMetaData", to_string(this->WithMetaData.value()));
    }

    if (this->WithVolatile.has_value()) {
      req.SetAttribute("WithVolatile", to_string(this->WithVolatile.value()));
    }

    return req;
  }
};

struct ListPeerNotes
{
  std::string NodeIdentifier;

  ListPeerNotes(std::string_view ident)
    : NodeIdentifier(ident)
  {
  }

  Req ToReq()
  {
    Req req("ListPeerNotes");

    req.SetAttribute("NodeIdentifier", this->NodeIdentifier);

    return req;
  }
};

struct AddPeer
{
  Node::Trust Trust;
  Node::Visibility Visibility;

  Req ToReq()
  {
    Req req("AddPeer");
    req.SetAttribute("Trust", to_string(this->Trust));
    req.SetAttribute("Visibility", to_string(this->Visibility));

    return req;
  }
};

struct ModifyPeer
{};

struct Disconnect
{
  Req ToReq() { return Req("Disconnect"); }
};

struct Shutdown
{
  Req ToReq() { return Req("Shutdown"); }
};

}

#endif // !FCP_REQUEST_HPP_
