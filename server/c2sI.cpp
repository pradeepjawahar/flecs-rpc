#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "c2sI.h"
#include "util.h"
#include "server-common.h"

using namespace std;



void C2SI::Get(
		const std::string& objID,
		FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "Get(" << objID << ")\n";

	_readfile((string(FleCS::Server::root_dir) + "/" + objID).c_str(), content);
}


void C2SI::Put(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "Put(" << objID << ")\n";

	_writefile((string(FleCS::Server::root_dir) + "/" + objID).c_str(), content);
}
