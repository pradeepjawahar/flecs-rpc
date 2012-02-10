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
	cout << "C2S::Get(" << objID << ")\n";

	_readfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);
}


void C2SI::Put(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "C2S::Put(" << objID << ")\n";

	_writefile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);
}


void C2SI::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "C2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);
}
