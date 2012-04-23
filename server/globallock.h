#ifndef __GLOBAL_LOCK_H__
#define __GLOBAL_LOCK_H__

#include <string>
#include <map>
#include <Ice/Ice.h>

#include "flecs.h"


struct LockID
{
	int server;	// server ID
	int id;		// per-server lock ID
};


class GlobalLock
{
public:
	GlobalLock(
			const std::string& path,
			const char type);

	~GlobalLock();

private:
	LockID _lock_id;
};


class GlobalLockMgr
{
public:
	static GlobalLockMgr& GetInstance();

	LockID Acquire(
			const std::string& path,
			const char type);

	void Release(LockID lid);


private:
	GlobalLockMgr();
	~GlobalLockMgr();

	std::map<std::string, IceUtil::Mutex*> _locks;

	std::vector<FleCS::SM2SPrx*> _lock_servers;
};


#endif
