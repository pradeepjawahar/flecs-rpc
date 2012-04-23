#include "globallock.h"
#include "server.h"

using namespace std;


GlobalLock::GlobalLock(
		const std::string& path,
		const char type)
{
	static GlobalLockMgr& glm = GlobalLockMgr::GetInstance();

	_lock_id = glm.Acquire(path, type);
}


GlobalLock::~GlobalLock()
{
	static GlobalLockMgr& glm = GlobalLockMgr::GetInstance();

	glm.Release(_lock_id);
}


GlobalLockMgr& GlobalLockMgr::GetInstance()
{
	static GlobalLockMgr glm;
	return glm;
}


int GlobalLockMgr::Acquire(
		const std::string& path,
		const char type)
{
	// TODO: select a lock server
	// TODO: acquire
	
	return 0;
}


void GlobalLockMgr::Release(int lock_id)
{
	// TODO: select a lock server
	// TODO: release
}


GlobalLockMgr::GlobalLockMgr()
{
	// get a list of lock servers from the master.  master needs a
	// lock for the operation. and after serving the first request,
	// the lock server list will remain unchanged.
	FleCS::MasterPrx& mp = FleCSServer::GetMasterProxy();
	std::vector<std::string> lock_servers = mp->GetLockServers();

	ostringstream oss;
	copy(lock_servers.begin(), lock_servers.end(), ostream_iterator<string>(oss, " "));
	_LOG(oss.str());
}
