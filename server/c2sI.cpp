#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "c2sI.h"
#include "util.h"
#include "server-common.h"

using namespace std;


static FleCS::Server1Prx& s2s_prx_meego(
		const Ice::Current& cur)
{
	static bool init = false;
	static FleCS::Server1Prx s2s_prx;

	if (init == false)
	{
		Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

		s2s_prx = FleCS::Server1Prx::checkedCast(
				ic
				->stringToProxy(string("s2s:") + "tcp -h meego -p 10000")
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


static FleCS::Server1Prx& s2s_prx_toothless(
		const Ice::Current& cur)
{
	static bool init = false;
	static FleCS::Server1Prx s2s_prx;

	if (init == false)
	{
		Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

		s2s_prx = FleCS::Server1Prx::checkedCast(
				ic
				->stringToProxy(string("s2s:") + "tcp -h toothless -p 10000")
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


// TODO: implement, i.e., ask to master server.
// 
// Get all server proxies except itself.
void _get_server_proxys(
		const Ice::Current& cur,
		vector<FleCS::Server1Prx*>& prxlist)
{
	prxlist.push_back(&s2s_prx_toothless(cur));
	prxlist.push_back(&s2s_prx_meego(cur));
}
// TODO: When to delete a proxy pointer? You don't know if it's in use or not,
// unless you lock the code that interacts with other servers, which would not
// scale in performance.
//
// Adding a node would be easy. After finishing the instantiation, its pointer can be added.


void Imp_c2s::Get(
		const std::string& objID,
		FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "C2S::Get(" << objID << ")\n";

	_readfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);
}


void Imp_c2s::Put(
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


void Imp_c2s::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current& cur)
{
	cout << "C2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);

	// propagate update to other servers.
	// TODO: maintain a server list, which is updated through a notification by a master node.
	// TODO: parallelize
	
	// TODO: M2S (master-to-server) service implementation.
	//
	// No concern of deadlock here as long as the master do not notify the originating server.
	//   server --(join)--> master --(notify)--> all other servers.
	// Master needs to make sure that join service is serialized.
	//
	// If there is only one thread that acts as dispatcher and worker, this may cause a deadlock.
	//   C1 ----> S1 --(S2 is not responding while processing C2's request)--> S2
	//   C2 ----> S2 --(S1 is not responding while processing C1's request)--> S1
	//
	
	// TODO: get server list, better yet get proxy list, excluding itself.
	// TODO: needs to be a separate module.
	vector<FleCS::Server1Prx*> prxlist;
	_get_server_proxys(cur, prxlist);
	
	// TODO: propagate update to all the others servers.
	for (vector<FleCS::Server1Prx*>::const_iterator i = prxlist.begin(); i != prxlist.end(); ++ i)
		(**i)->Append(objID, content);
}
