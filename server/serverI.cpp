#include <Ice/Ice.h>

#include "serverI.h"
#include "util.h"
#include "server-common.h"
#include "container.h"

using namespace std;


void ServerI::Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.Server_Put(bucketID, objID, content);
}


void ServerI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.Server_Append(bucketID, objID, content);
}


void ServerI::ServerJoined(
		const std::string& endpoint,
		const Ice::Current& cur)
{
	_LOG(endpoint);

	FleCS::ServerImpl::AddServer(endpoint, cur.adapter->getCommunicator());
}


void ServerI::ServerLeft(
		const std::string& endpoint,
		const Ice::Current&)
{
	_LOG(endpoint);
}
