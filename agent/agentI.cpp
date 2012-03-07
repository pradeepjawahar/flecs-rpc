#include <boost/date_time/posix_time/posix_time.hpp>

#include "agentI.h"
#include "agent-common.h"

using namespace std;


string cur_datetime()
{
	ostringstream oss;

	const boost::posix_time::ptime now =
		boost::posix_time::second_clock::local_time();

	boost::posix_time::time_facet* const f =
		      new boost::posix_time::time_facet("%m%d-%H%M%S");

	oss.imbue(locale(oss.getloc(), f));

	oss << now;

	return oss.str();
}


void AgentI::Exec(
		const std::string& cmd_,
		int& ret,
		std::string& logFile,
		const Ice::Current&)
{
	IceUtil::Mutex::Lock lock(_lock);

	_LOG(cmd_);

	// Allow only one instance of this function by employing a lock?
	// This function itself may block while executing... let's see.

	logFile = "/dev/shm/flecs-log/agent-" + cur_datetime();
	string cmd = "echo \"" + cmd_ + "\n\" >> logFile; "
		"( " + cmd_ + ") >> " + logFile + " 2>&1";

	ret = system(cmd.c_str());
}
