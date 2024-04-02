#ifndef FCPRESULT_H__
#define FCPRESULT_H__

#include <vector>
#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "JobTicket.h"
#include "Base64.h"

namespace FCPLib {

class TestDDAReplyResponse {
  friend struct TestDDAReplyConverter;

  Message::Ptr message;

  TestDDAReplyResponse(Message::Ptr message_) : message(message_) {}

  const std::string& getField(std::string field) const {
    const std::string& ret = message->getField(field);
    if (ret == "")
      throw std::logic_error("TestDDAReply does not contain " + field + " field.");
    return ret;
  }
public:
  typedef boost::shared_ptr<TestDDAReplyResponse> Ptr;

  const std::string getDirectory() const {
    return message->getField("Directory");
  }
  const std::string getReadFilename() const {
    return getField("ReadFilename");
  }
  const std::string getWriteFilename() const {
    return getField("WriteFilename");
  }
  const std::string getContent () const {
    return getField("ContentToWrite");
  }
};

class TestDDAResponse {
public:
  TestDDAResponse(std::string dir, bool read, bool write)
    : directory(dir),
      readDirectory(read),
      writeDirectory(write)
  {}
  std::string directory;
  bool readDirectory;
  bool writeDirectory;
};

class PeerNote {
public:
  PeerNote( Message::Ptr m )
    : nodeIdentifier( m->getField("NodeIdentifier") ),
      noteText( Base64::base64Decode( m->getField("NoteText") ) ),
      peerNoteType( boost::lexical_cast<int>(m->getField("PeerNoteType")) )
  {}
  std::string nodeIdentifier;
  std::string noteText;
  int peerNoteType;

  std::string toString() const {
    return "NodeIdentifier="+nodeIdentifier+"\n"+
           "NodeText="+noteText+"\n"+
           "PeerNoteType="+boost::lexical_cast<std::string>(peerNoteType)+"\n";
  }
};

typedef std::vector<PeerNote> PeerNoteContainer;

///////////

struct GetMessage {
  Message::Ptr
  operator()( ServerMessage::Ptr sm ){
    return sm->getMessage();
  }
};

struct MessageConverter {
  Message::Ptr
  operator()( Response &resp )
  {
    return Message::Ptr( resp.front()->getMessage() );
  }
};

struct LastMessageConverter {
  ServerMessage::Ptr
  operator()( Response &resp )
  {
    return ServerMessage::Ptr( resp.back() );
  }
};

struct VectorConverter {
  MessagePtrContainer
  operator()( Response &resp )
  {
    MessagePtrContainer ret( resp.size() ) ;
    transform( resp.begin(), resp.end(), ret.begin(), GetMessage() );
    return ret;
  }
};

struct VectorWithoutLastConverter {
  MessagePtrContainer
  operator()( Response &resp )
  {
    int numToCopy = resp.size() - 1;
    MessagePtrContainer ret = MessagePtrContainer( numToCopy );
    transform( resp.begin(), resp.begin() + numToCopy, ret.begin(), GetMessage() );
    return ret;
  }
};

struct TestDDAReplyConverter {
  TestDDAReplyResponse::Ptr
  operator()( Response &resp )
  {
    return TestDDAReplyResponse::Ptr( new TestDDAReplyResponse(resp.front()->getMessage()) );
  }
};

struct PeerNotesConverter {
  PeerNoteContainer
  operator()( Response &resp )
  {
    PeerNoteContainer ret;
    int size = resp.size() - 1;
    Response::iterator end = resp.begin() + size;
    for (Response::iterator it = resp.begin(); it != end; ++it)
      ret.push_back( PeerNote( (*it)->getMessage() ) );
    return ret;
  }
};

struct PeerNoteConverter {
  PeerNote
  operator()( Response &resp )
  {
    return PeerNote( resp.front()->getMessage() );
  }
};


//////////

template<typename ReturnT, typename ConverterT>
ReturnT createResult( Response &resp )
{
  return ConverterT()( resp );
}

}

#endif
