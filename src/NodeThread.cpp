

#include <ctime>
#include <boost/lexical_cast.hpp>

#include "NodeThread.h"
#include <spdlog/spdlog.h>
#include "Node.h"

using namespace FCPLib;
using namespace ZThread;

NodeThread::NodeThread(Node* n,
                       std::string &host,
                       int port,
                       JobTicketQueuePtr clientReqQueue_) throw()
  : node(n),
    clientReqQueue(clientReqQueue_),
    host_(host),
    port_(port),
    s(new Server( host_, port_ ))
{
}

void NodeThread::run(){
  ServerMessage::Ptr m;
  JobTicket::Ptr job;
  spdlog::info("FCPNode: manager thread starting");
  try {
    while (!Thread::interrupted()) {
      //check for incoming message from node
      spdlog::trace("_mgrThread: Testing for incoming message");
      if (s->dataAvailable()){
        spdlog::debug("_mgrThread: Retrieving incoming message");
        m = ServerMessage::factory(s);
        spdlog::debug("_mgrThread: Got incoming message, dispatching");
        // dispatch the message
        doMessage(m);
      }
      //check for incoming message from client
      spdlog::trace("_mgrThread: Testing for incoming req");
      if (!clientReqQueue->empty()){
        spdlog::debug("_mgrThread: Got incoming client req");
        job = clientReqQueue->get();
        spdlog::debug("_mgrThread: Got incoming client req from the queue");
        spdlog::trace(job->toString());
        sendClientReq(job);
      }
      Thread::sleep(100);  // do I need this?
    }
  } catch (ZThread::Synchronization_Exception& e) {
    // thread was interupted, normal way to shutdown the thread
    // this object will be destroyed
    spdlog::error("_mgrThread: Caught Synchronization_Exception: {}", e.what());
    node->setIsAlive(false);
  } catch (std::runtime_error& e) {
    // some error has occured, keep the thread so you can access the isAlive and getFailure
    spdlog::error("_mgrThread: Caught std::runtime_error: {}", e.what());
    node->setIsAlive( false );
    node->setFailure( e.what() );
  } catch (std::exception& e) {
    // some error has occured, keep the thread so you can access the isAlive and getFailure
    spdlog::error("_mgrThread: Caught std::exception: {}", e.what());
    node->setIsAlive( false );
    node->setFailure( e.what() );
  } catch (...) {
    // thread is stopped and we don't know what has happend
    spdlog::error("_mgrThread: Caught something else");
    node->setIsAlive(false);
    node->setFailure( "unknown error" );
  }
}

void
NodeThread::sendClientReq(JobTicket::Ptr job)
{
  spdlog::trace("sendClientReq : top");
  if (job->getCommandName() != "WatchGlobal") {
    spdlog::trace("sendClientReq : about to add the job to the map");
    jobs[job->isGlobal() ? 1 : 0][job->getId()] = job;
    spdlog::trace("sendClientReq : added the job to the map");
  }

  s->send(job->getCommand());
  job->timeQueued = (unsigned int) time(0);
}

void
NodeThread::doMessage(ServerMessage::Ptr message)
{
  JobTicket::Ptr job;
  std::map<std::string, JobTicket::Ptr>::iterator it;

  std::string tmp = message->getMessage()->getField("Global");
  tmp = tmp == "" ? "false" : tmp;
  int isGlobal = tmp == "false" ? 0 : 1;

  it = jobs[isGlobal].find(message->getIdOfJob());
  if (it == jobs[isGlobal].end()) {
    spdlog::info("doMessage : received {}, cannot find {} in started jobs", message->getMessage()->getHeader(), message->getIdOfJob());
    /// message from global queue or error
    Message::Ptr m = message->getMessage();
    if (!isGlobal) { // error
      spdlog::debug("doMessage : received error message");
      // TODO: create a mean of passing error messages to client programme
      return;
    } else { // global queue, create a job
      spdlog::debug("doMessage : received message from a global queue");
      if ( m->getField("Identifier") == "" ) {
        // should never happen
        spdlog::error("doMessage : global message does not contain identifier !???");
        return;
      }
      JobTicket::Ptr job = JobTicket::factory( this->node, m->getField("Identifier"), m );
      job->setGlobal(isGlobal).setPersistent(true);
      jobs[isGlobal][m->getField("Identifier")] = job;
      return;
    }
  }

  job = it->second;

  if ( message->isLast( job ) ) {
    spdlog::trace("doMessage : last message for the job");
    job->putResponse(1, message);
    job->putResult();

    if (!job->keep) {
      spdlog::trace("doMessage : job should not be kept, erasing");
      jobs[isGlobal].erase( it );
    }
  }
  else {
    job->putResponse(0, message);
  }
}

