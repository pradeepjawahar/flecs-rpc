#ifndef __GLOBAL_LOCK_H__
#define __GLOBAL_LOCK_H__

#include <string>
#include <map>
#include <Ice/Ice.h>


class GlobalLock
{
public:
	GlobalLock(
			const std::string& path,
			const char type);

	~GlobalLock();

private:
	int _lock_id;
};


class GlobalLockMgr
{
public:
	static GlobalLockMgr& GetInstance();

	int Acquire(
			const std::string& path,
			const char type);

	void Release(int lock_id);


private:
	GlobalLockMgr();

	std::map<std::string, IceUtil::Mutex*> _locks;

	std::vector<std::string> _lock_servers;
};


#endif
