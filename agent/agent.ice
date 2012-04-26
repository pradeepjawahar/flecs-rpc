#ifndef __AGENT_ICE__
#define __AGENT_ICE__

module Common
{
	interface Agent
	{
		void Exec(string cmd, out int ret, out string logFile);
	};
};

#endif
