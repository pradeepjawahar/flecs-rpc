#include <dlfcn.h>

#include <stdexcept>

#include "container.h"
#include "server.h"
#include "util.h"

using namespace std;



long Container::C2S_Size(
		const std::string& bucketID,
		const std::string& objID)
{
	_LOG(objID);

	return _file_size(FleCSServer::stg_root_dir + "/" + bucketID + "/" +objID);
}


long Container::S2S_Size(
		const std::string& bucketID,
		const std::string& objID)
{
	_LOG(objID);

	return _file_size(FleCSServer::stg_root_dir + "/" + bucketID + "/" +objID);
}


ContainerMgr& ContainerMgr::GetInstance()
{
	static ContainerMgr cm;
	return cm;
}


Container& ContainerMgr::GetContainer(const std::string& container_name)
{
	map<string, Plugin>::iterator i = _containers.find(container_name);

	if (i == _containers.end())
	{
		// Multiple threads can be here simultaneously.

		// acquire a lock.
		IceUtil::Mutex::Lock _(_container_load_lock);
		
		// Make sure no other thread have loaded the container before.
		i = _containers.find(container_name);
		if (i == _containers.end())
		{
			_LoadContainerPlugin(container_name);

			i = _containers.find(container_name);
			if (i == _containers.end())
				throw runtime_error("unexpected!");
		}
	}

	return *(i->second._container);
}


ContainerMgr::ContainerMgr()
{
}


ContainerMgr::~ContainerMgr()
{
	// deallocate container instances and unload plugins.

	for (std::map<std::string, Plugin>::iterator i = _containers.begin(); i != _containers.end(); ++ i)
	{
		delete i->second._container;

		if (dlclose(i->second._handle) != 0)
			throw runtime_error(string("error in dlclose(): ") + dlerror());
	}

	_containers.clear();
}


void ContainerMgr::_LoadContainerPlugin(const std::string& container_name)
{
	const char* funcname = "CreateInstance";

	string filename = "libflecs-container-" + container_name + ".so";

	Plugin plugin;

	if ((plugin._handle = dlopen(filename.c_str(), RTLD_LAZY)) == NULL)
		throw runtime_error(string("error in dlopen file ") + filename 
				+ ": " + dlerror());

	// clear any existing error
	dlerror();

	Container* (*p_createinstance)() = reinterpret_cast<Container* (*)()>(dlsym(plugin._handle, funcname));

	char* error = dlerror();
	if (error != NULL)
		throw runtime_error(string("error in dlsym(): ") + error);

	// Create instance
	plugin._container = p_createinstance();

	// Initialize
	plugin._container->Init();

	_containers[container_name] = plugin;

	_LOG("Loaded " << container_name);
}
