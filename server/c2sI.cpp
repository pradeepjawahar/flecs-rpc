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

	_readfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + objID).c_str(), content);
}


// TODO: parallelize Put() and Append()
void C2SI::Put(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "C2S::Put(" << objID << ")\n";

	_writefile((string(FleCS::ServerImpl::stg_root_dir) + "/" + objID).c_str(), content);

	// propagate update to the other servers.
	map<string, FleCS::Server1Prx*>& s = FleCS::ServerImpl::_servers;
	for (map<string, FleCS::Server1Prx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
	{
		(*(i->second))->Append(objID, content);
	}
}


void C2SI::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current& cur)
{
	cout << "C2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + objID).c_str(), content);

	// propagate update to the other servers.
	//
	// If there is only one thread that acts as dispatcher and worker, this may cause a deadlock.
	//   C1 ----> S1 --(S2 is not responding while processing C2's request)--> S2
	//   C2 ----> S2 --(S1 is not responding while processing C1's request)--> S1
	//
	// The size of thread pool seems to be large, thus the above won't happen.
	
	map<string, FleCS::Server1Prx*>& s = FleCS::ServerImpl::_servers;
	for (map<string, FleCS::Server1Prx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
	{
		(*(i->second))->Append(objID, content);
	}
}
