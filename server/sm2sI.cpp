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

	c.S2S_Put(bucketID, objID, content);
}


void SM2SI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.S2S_Append(bucketID, objID, content);
}


void SM2SI::Delete(
		const std::string& bucketID,
		const std::string& objID,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.S2S_Delete(bucketID, objID);
}


void SM2SI::ServerJoined(
		const std::string& endpoint,
		const Ice::Current& cur)
{
	_LOG(endpoint);

	FleCSServer::AddServer(endpoint);
}


void SM2SI::ServerLeft(
		const std::string& endpoint,
		const Ice::Current&)
{
	_LOG(endpoint);
}


void SM2SI::AcquireLock(
		const std::string& lockPath,
		Ice::Byte type,
		const std::string& hostname,
		long tid,
		const Ice::Current&)
{
	_LOG(lockPath << " " << type << " " << hostname << " " << tid);

	Mutex* m = NULL;

	{
		// allocate a lock or get an existing lock, incrementing reference
		// count.
		
		IceUtil::Mutex::Lock _(_locks_lock);

		map<string, Mutex*>::iterator i = _locks.find(lockPath);

		if (i != _locks.end())
		{
			m = i->second;
		}
		else
		{
			// maintaining a lock pool would be a little better for this, so
			// that it does not involve frequent news and deletes. It should
			// not be performance-critical since network cost would dominate in
			// a distributed lock.

			m = new Mutex;
			_locks[lockPath] = m;
		}

		// *at least* ref_cnt number of clients are trying to acqure the lock.
		m->ref_cnt ++;
	}

	// TODO: differentiate R from W.
	m->lock.lock();

	_LOG(lockPath << " " << type << " " << hostname << " " << tid);
}


void SM2SI::ReleaseLock(
		const std::string& lockPath,
		Ice::Byte type,
		const std::string& hostname,
		long tid,
		const Ice::Current&)
{
	_LOG(lockPath << " " << type << " " << hostname << " " << tid);

	IceUtil::Mutex::Lock _(_locks_lock);

	map<string, Mutex*>::iterator i = _locks.find(lockPath);
	if (i == _locks.end())
		throw runtime_error(string("Could not find lock: ") + lockPath);

	// TODO: differentiate R from W.
	i->second->lock.unlock();

	// decrese ref_cnt, and if it's 0, delete the lock.

	i->second->ref_cnt --;
	if (i->second->ref_cnt == 0)
	{
		delete i->second;
		_locks.erase(i);
	}
}
