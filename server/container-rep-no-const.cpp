#include "container.h"
#include "util.h"

using namespace std;


class rep_no_const : public Container
{
public:
	void Init()
	{
		_LOG("");
	}
	

	void C2S_Get(
		const std::string& bucketID,
		const std::string& objID,
		FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_readfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}
	

private:
};


extern "C" rep_no_const* CreateInstance()
{
	return new rep_no_const;
}
