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
	const std::string _path;
	const char _type;
};


class GlobalLockMgr
{
public:
	static GlobalLockMgr& GetInstance();

	void Acquire(
			const std::string& path,
			const char type);

	void Release(
			const std::string& path,
			const char type);


private:
	GlobalLockMgr();
	~GlobalLockMgr();

	std::string _hostname;

	std::map<std::string, IceUtil::Mutex*> _locks;

	std::vector<FleCS::SM2SPrx*> _lock_servers;
};


#endif
