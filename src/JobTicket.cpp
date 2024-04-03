#include "JobTicket.h"
#include "DefaultValues.h"
#include <spdlog/spdlog.h>

#include <boost/lexical_cast.hpp>
#include "Utils.h"
#include "Node.h"


using namespace FCPLib;

JobTicket::Ptr
JobTicket::factory(Node* n, std::string id, Message::Ptr cmd)
{
    spdlog::trace("Creating {}", cmd->getHeader());
   Ptr ret( new JobTicket() );
   ret->init(n, id, cmd);

   return ret;
}

void
JobTicket::init(Node* n, std::string &id, Message::Ptr cmd)
{
   this->node = n;
   this->id = id;
   this->cmd = cmd;

   this->lock.acquire();
   this->reqSentLock.acquire();

   spdlog::debug(this->toString());
}

void
JobTicket::wait(unsigned int timeout)
{
  if (!timeout){
    while (!lock.tryAcquire(100)) {
      if (! node->isAlive() ) {
        if ( node->hasFailure() ) throw node->getFailure();
        else throw FCPNodeFailure( "node thread is not alive" );
      }
      ZThread::Thread::sleep(100);
    }
    lock.release();
    return;
  }

  unsigned int then = (unsigned int) time(0);
  unsigned int elapsed;
  while (!reqSentLock.tryAcquire(100)){
    if (! node->isAlive() ) {
      if ( node->hasFailure() ) throw node->getFailure();
      else throw FCPNodeFailure( "node thread is not alive" );
    }
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout){
      ZThread::Thread::sleep(1000);
      spdlog::debug("wait:{}:{}: job not dispatched, timeout in {}",
        this->getCommandName(), this->getId(), timeout-elapsed);
      continue;
    }
    spdlog::debug("wait:{}:{}: timeout on send command", this->getCommandName(), this->getId());

    throw CommandTimeout("Timeout sending " + this->getCommandName());
  }
  spdlog::debug("wait:{}:{}: job now dispatched", this->getCommandName(), this->getId());
  while (!lock.tryAcquire(100)){
    if (! node->isAlive() ) {
      if ( node->hasFailure() ) throw node->getFailure();
      else throw FCPNodeFailure( "node thread is not alive" );
    }
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout) {
      ZThread::Thread::sleep(2000);
      spdlog::debug("wait:{}:{}: waiting for node response, timeout in {}", this->getCommandName(), this->getId(), timeout-elapsed);
      continue;
    }
    spdlog::debug("wait:{}:{}: timeout on node response", this->getCommandName(), this->getId());
    throw CommandTimeout("Timeout sending " + this->getCommandName());
  }
  spdlog::debug("wait:{}:{}: job complete", this->getCommandName(), this->getId());
  lock.release();
}

void
JobTicket::waitTillReqSent(unsigned int timeout)
{
  unsigned int then = (unsigned int) time(0);
  unsigned int elapsed;
  while (!reqSentLock.tryAcquire(100)){
    if (! node->isAlive() ) {
      if ( node->hasFailure() ) throw node->getFailure();
      else throw FCPNodeFailure( "node thread is not alive" );
    }
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout){
      ZThread::Thread::sleep(1000);
      spdlog::debug("wait:{}:{}: job not dispatched, timeout in {}", this->getCommandName(), this->getId(), timeout-elapsed);
      continue;
    }
    spdlog::debug("wait:{}:{}: timeoit on send command", this->getCommandName(), this->getId());
    throw CommandTimeout("Timeout sending " + this->getCommandName());
  }
}

const std::string&
JobTicket::toString()
{
  if (isReprValid)
    return repr;

  repr = "";
  isReprValid = true;

  repr += "Job id=" + id +
             " keepJob=" + Converter::toString( keep ) +
             " global=" + Converter::toString( global ) +
             " persistent=" + Converter::toString( persistent ) + "\n";

  repr += "Message=" + cmd->getHeader();

  return repr;
}

void
JobTicket::putResult()
{
  ZThread::Guard<ZThread::Mutex> g(access);
  _isFinished = true;
  lock.release();
}

GetJob::Ptr
GetJob::factory(Node *n, std::string id, Message::Ptr cmd)
{
  spdlog::trace("Creating {}", cmd->getHeader());
  Ptr ret( new GetJob() );
  ret->init(n, id, cmd);

  return ret;
}

const std::string&
GetJob::toString()
{
  if (isReprValid)
    return repr;

  repr = "";
  isReprValid = true;

  repr += "Job id=" + id + " " +
             " keepJob=" + Converter::toString( keep ) +
             " global=" + Converter::toString( global ) +
             " persistent=" + Converter::toString( persistent ) +
             " returnType=" + boost::lexical_cast<std::string>( retType )+ "\n";

  repr += "Message=" + cmd->getHeader();

  return repr;
}
