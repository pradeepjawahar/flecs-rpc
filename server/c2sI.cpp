#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "c2sI.h"
#include "util.h"
#include "server-common.h"

using namespace std;


static FleCS::S2SPrx& s2s_prx(
		const Ice::Current& cur)
{
	static bool init = false;
	static FleCS::S2SPrx s2s_prx;

	if (init == false)
	{
		Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

		s2s_prx = FleCS::S2SPrx::checkedCast(
				ic
				->propertyToProxy("FleCS_s2s.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!s2s_prx)
		{
			cerr << "invalid proxy" << endl;
			exit(EXIT_FAILURE);
		}

		init = true;
	}

	return s2s_prx;
}


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

	// TODO: propagate update to other servers.
	
	// Where to get the server list? Parameterize later.
	
	// Need to maintain a proxy list.
}


void C2SI::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current& cur)
{
	cout << "C2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);

	// propagate update to other servers.
	// TODO: parameterize server list.
	s2s_prx(cur)->Append(objID, content);
}
