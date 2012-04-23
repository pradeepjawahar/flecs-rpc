#include "c2sI.h"
#include "util.h"
#include "container.h"

using namespace std;


void C2SI::Get(
		const std::string& bucketID,
		const std::string& objID,
		FleCS::ByteSeq& content,
		const Ice::Current&)
{
	// Depending on the bucketID, a proper container code needs to be
	// invoked.  For quick prototyping, for now, there is only one bucket
	// (instance of a container) per container. It can be easily extended
	// to allow multiple buckets per container with like this:
	// /storage_root/container_type/bucket_name/obj_path/...
	
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.C2S_Get(bucketID, objID, content);
}


void C2SI::Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.C2S_Put(bucketID, objID, content);
}


void C2SI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.C2S_Append(bucketID, objID, content);
}
