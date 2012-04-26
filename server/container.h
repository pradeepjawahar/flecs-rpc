#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <string>
#include <map>

#include "flecs.h"


class Container
{
public:
	virtual void Init() = 0;
	
	virtual void C2S_Get(
		const std::string& bucketID,
		const std::string& objID,
		FleCS::ByteSeq& content) = 0;

	virtual void C2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content) = 0;

	virtual void C2S_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content) = 0;

	virtual void S2S_Get(
                const std::string& bucketID,
                const std::string& objID,
                FleCS::ByteSeq& content) = 0;


	virtual void S2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content) = 0;

	virtual void S2S_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content) = 0;
};


class ContainerMgr
{
public:
	static ContainerMgr& GetInstance();

	Container& GetContainer(const std::string& container_name);


protected:
	ContainerMgr();

	~ContainerMgr();


private:
	struct Plugin
	{
		void* _handle;
		Container* _container;
	};


	void _LoadContainerPlugin(const std::string& container_name);


	std::map<std::string, Plugin> _containers;

	IceUtil::Mutex _container_load_lock;
};


#endif	// __CONTAINER_H__
