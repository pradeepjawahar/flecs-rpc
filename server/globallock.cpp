#include "globallock.h"
#include "server.h"

#include <boost/functional/hash.hpp>
#include <boost/asio/ip/host_name.hpp>

using namespace std;


GlobalLock::GlobalLock(
		const std::string& path,
		const char type)
: _path(path), _type(type)
{
	static GlobalLockMgr& glm = GlobalLockMgr::GetInstance();

	glm.Acquire(_path, _type);
}


GlobalLock::~GlobalLock()
{
	static GlobalLockMgr& glm = GlobalLockMgr::GetInstance();

	glm.Release(_path, _type);
}


GlobalLockMgr& GlobalLockMgr::GetInstance()
{
	static GlobalLockMgr glm;
	return glm;
}


void GlobalLockMgr::Acquire(
		const std::string& path,
		const char type)
{
	// select a lock server
	boost::hash<string> string_hash;
	int s = string_hash(path) % _lock_servers.size();

	// acquire a lock
	IceUtil::ThreadControl self;
	unsigned long tid = self.id();
	(*_lock_servers[s])->AcquireLock(path, type, _hostname, tid);
}


void GlobalLockMgr::Release(
		const std::string& path,
		const char type)
{
	// select the lock server
	boost::hash<string> string_hash;
	int s = string_hash(path) % _lock_servers.size();

	// release the lock
	IceUtil::ThreadControl self;
	unsigned long tid = self.id();
	(*_lock_servers[s])->ReleaseLock(path, type, _hostname, tid);
}


GlobalLockMgr::GlobalLockMgr()
{
	_hostname = boost::asio::ip::host_name();

	// Get a list of lock servers from the master.  master needs a lock for the
	// operation. and after serving the first request, the lock server list
	// will remain unchanged.
	
	FleCS::MasterPrx& mp = FleCSServer::GetMasterProxy();
	vector<string> lsvrs = mp->GetLockServers();

	ostringstream oss;
	copy(lsvrs.begin(), lsvrs.end(), ostream_iterator<string>(oss, " "));
	_LOG(oss.str());

	// Prepare lock server proxies. A separate server proxy list from the one
	// at the server module is maintained here.
	// 
	// The idea of reusing the peer proxies (peer_servers) in the server module
	// has been dropped, since it lacks the server itself (the same server with
	// the lock requester).  Adding *self* to the peer_servers would complicate
	// the update propagation and freeing of proxies.
	//
	// Would freeing a proxy affect another proxy that points the same server?
	// It should be fine, since Proxy is a handle that reference-counts an
	// object. Deallocation of the object will occur when the last reference
	// goes away.
	
	for (vector<string>::iterator i = lsvrs.begin(); i != lsvrs.end(); ++ i)
		_lock_servers.push_back(_new_sm2s_proxy(*i));
}


GlobalLockMgr::~GlobalLockMgr()
{
	for (vector<FleCS::SM2SPrx*>::iterator i = _lock_servers.begin(); i != _lock_servers.end(); ++ i)
		delete *i;
}
