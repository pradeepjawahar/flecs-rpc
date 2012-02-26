#ifndef __AGENT_I_H__
#define __AGENT_I_H__

#include "agent.h"

class AgentI : public Common::Agent
{
public:
    virtual void Exec(
			const std::string& cmd,
			int& ret,
			std::string& logFile,
			const Ice::Current&);
};


#endif
