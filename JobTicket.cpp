#include "JobTicket.h"
#include "DefaultValues.h"
#include "Log.h"

#include <boost/lexical_cast.hpp>
#include "Utils.h"


using namespace FCPLib;

JobTicket::Ptr
JobTicket::factory(std::string id, Message::Ptr cmd)
{
   log().log(NOISY, "Creating " + cmd->getHeader());
   Ptr ret( new JobTicket() );

   ret->id = id;
   ret->cmd = cmd;

   ret->lock.acquire();
   ret->reqSentLock.acquire();

   log().log(DEBUG, ret->toString());
   return ret;
}

void
JobTicket::wait(unsigned int timeout)
{
  if (!timeout){
    while (!lock.tryAcquire(100))
      ZThread::Thread::sleep(100);
    lock.release();
    return;
  }

  unsigned int then = (unsigned int) time(0);
  unsigned int elapsed;
  while (!reqSentLock.tryAcquire(100)){
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout){
      ZThread::Thread::sleep(1000);
      log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": job not dispatched, timeout in " +
        boost::lexical_cast<std::string>(timeout-elapsed));
      continue;
    }
    log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": timeout on send command");

    throw CommandTimeout("Timeout sending " + this->getCommandName());
  }
  log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": job now dispatched");
  while (!lock.tryAcquire(100)){
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout) {
      ZThread::Thread::sleep(2000);
      log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": waiting for node response, timeout in " +
        boost::lexical_cast<std::string>(timeout-elapsed));
      continue;
    }
    log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": timeout on node response");

    throw CommandTimeout("Timeout sending " + this->getCommandName());
  }
  log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": job complete");
  lock.release();
}

void
JobTicket::waitTillReqSent(unsigned int timeout)
{
  unsigned int then = (unsigned int) time(0);
  unsigned int elapsed;
  while (!reqSentLock.tryAcquire(100)){
    elapsed = (unsigned int) time(0) - then;
    if (elapsed < timeout){
      ZThread::Thread::sleep(1000);
      log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": job not dispatched, timeout in " +
        boost::lexical_cast<std::string>(timeout-elapsed));
      continue;
    }
    log().log(DEBUG, "wait:"+this->getCommandName()+":"+this->getId()+": timeout on send command");
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

  repr += "Job id=" + id + " " +
             " keepJob=" + Converter::toString( keep ) + "\n";
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
