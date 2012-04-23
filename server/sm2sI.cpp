#include <Ice/Ice.h>

#include "sm2sI.h"
#include "util.h"
#include "container.h"
#include "server.h"

using namespace std;


void SM2SI::Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.Server_Put(bucketID, objID, content);
}


void SM2SI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.Server_Append(bucketID, objID, content);
}


void SM2SI::ServerJoined(
		const std::string& endpoint,
		const Ice::Current& cur)
{
	_LOG(endpoint);

	FleCSServer::AddServer(endpoint, cur.adapter->getCommunicator());
}


void SM2SI::ServerLeft(
		const std::string& endpoint,
		const Ice::Current&)
{
	_LOG(endpoint);
}
