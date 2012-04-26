#ifndef __MASTER_I_H__
#define __MASTER_I_H__

#include <string>
#include <set>
#include <map>

#include "flecs.h"


class MasterI : public FleCS::Master
{
public:
    virtual void Join(
			const std::string& endpoint,
			std::vector<std::string>& existingServers,
			const Ice::Current&);

    virtual std::vector<std::string> GetLockServers(
			const Ice::Current&);

private:
	std::map<std::string, FleCS::SM2SPrx*> _servers;

	// Serialize all Join and GetLockServers requests.
	IceUtil::Mutex _lock;
};


#endif
