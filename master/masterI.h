#ifndef __MASTER_I_H__
#define __MASTER_I_H__

#include <string>
#include <set>
#include <map>

#include "flecs.h"


class MasterI : public FleCS::Master
{
public:
    virtual void ServerJoin(
			const std::string& endpoint,
			const Ice::Current&);

private:
	std::map<std::string, FleCS::Server1Prx*> _servers;

	IceUtil::Mutex _lock;
};


#endif
