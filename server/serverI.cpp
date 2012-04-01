#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "serverI.h"
#include "util.h"
#include "server-common.h"

using namespace std;


void ServerI::Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	_LOG(objID);

	_writefile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
}


void ServerI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	_LOG(objID);

	_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
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
