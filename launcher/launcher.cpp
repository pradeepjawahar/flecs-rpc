#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/select.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <log4cxx/logger.h>

using namespace std;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("flecs.launcher"));
#define _LOG(A) LOG4CXX_INFO(logger, A)


ssize_t readn(int fd, void* buf, size_t count)
{
	ssize_t bytes_read = 0;

	ssize_t n;
	
	while (1)
	{
		n = read(fd, (void*) (static_cast<char*>(buf) + bytes_read), count - bytes_read);
		if (n == -1)
		{
			perror("read");
			exit(1);
		}

		bytes_read += n;

		if (bytes_read == (ssize_t) count)
			return bytes_read;

		if (n == 0)
			return -1;	// reached EOF before reading count bytes.
	}
}


class Watcher
{
public:
	struct WatchEntry
	{
		WatchEntry()
		{
		}

		WatchEntry(const int wd_,
				const string& filename_,
				const string& cmd_) :
			wd(wd_),
			filename(filename_),
			cmd(cmd_)
		{
		}

		int wd;
		string filename;
		string cmd;
	};


public:
	Watcher()
	{
		_fd = inotify_init();
		if (_fd == -1)
		{
			perror("inotify_init");
			exit(1);
		}
	}


	~Watcher()
	{
		if (close(_fd) == -1)
		{
			perror("close");
			exit(1);
		}
	}


	void Add(const char* filename, const char* cmd)
	{
		int wd = inotify_add_watch(_fd, filename, IN_CLOSE_WRITE);
		if (wd == -1)
		{
			perror("inotify_add_watch");
			exit(1);
		}

		WatchEntry we(wd, filename, cmd);
		_wds[wd] = we;
		_LOG(we);
	}


	void Watch()
	{
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(_fd, &read_fds);

		struct inotify_event ie;
		while (true)
		{
			if (select(_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
			{
				perror("select");
				exit(1);
			}

			if (! FD_ISSET(_fd, &read_fds))
			{
				fprintf(stderr, "FD_ISSET()\n");
				exit(1);
			}

			readn(_fd, &ie, sizeof(struct inotify_event));
			//print(ie);

			_Launch(ie.wd);

			if (ie.mask & IN_IGNORED)
				_ResetWatcher();
		}
	}

	
private:
	void _ResetWatcher()
	{
		map<int, WatchEntry> old_wds = _wds;
		_wds.clear();

		for (map<int, WatchEntry>::iterator i = old_wds.begin(); i != old_wds.end(); ++ i)
		{
			int wd = inotify_add_watch(_fd, i->second.filename.c_str(), IN_CLOSE_WRITE);
			if (wd == -1)
			{
				perror("inotify_add_watch");
				exit(1);
			}

			WatchEntry we(wd, i->second.filename, i->second.cmd);
			_wds[wd] = we;
			_LOG(we);
		}
	}


	void _Launch(int wd)
	{
		_LOG("Launching " << _wds[wd].filename);

		int ret = system(_wds[wd].cmd.c_str());

		if (ret == -1)
			_LOG("exec failed.");
		if (WIFSIGNALED(ret))
			_LOG("signaled: " << WTERMSIG(ret));
		if (WEXITSTATUS(ret) != 0)
			_LOG("exit status: " << WEXITSTATUS(ret));
	}


	int _fd;
	map<int, WatchEntry> _wds;	// mapping of watch descriptor to file name.
};


ostream& operator<< (ostream& os, const Watcher::WatchEntry& e)
{
	os << e.wd << " " << e.filename << " \"" << e.cmd << "\"";

	return os;
}


//void print(const struct inotify_event& ie)
//{
//	printf("%d %u %u %u [%s]\n",
//			ie.wd,		// int wd; Watch descriptor
//			ie.mask,	// uint32_t mask; Mask of events
//			ie.cookie,	// uint32_t cookie; Unique cookie associating related events (for rename(2))
//			ie.len,		// uint32_t len; Size of 'name' field
//			ie.len > 0 ? ie.name : NULL); // char name[]; Optional null-terminated name
//}


int main()
{
	cout << "launcher starting...\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("launcher starting...");

	Watcher w;
	w.Add("flecs.trigger.agent", "export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64; cd /dev/shm/work/flecs-rpc/.build/agent; ./flecs-agent-server;");
	w.Add("flecs.trigger.master", "export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64; cd /dev/shm/work/flecs-rpc/.build/master; ./flecs-master;");
	w.Add("flecs.trigger.server", "export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64; cd /dev/shm/work/flecs-rpc/.build/server; ./flecs-server;");
	w.Add("flecs.trigger.client", "export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64; cd /dev/shm/work/flecs-rpc/.build/client; ./flecs-client;");

	w.Watch();

	_LOG("launcher ended.");

	return 0;
}
