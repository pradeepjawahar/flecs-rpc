#include "server.h"

using namespace std;


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	parse_args(argc, argv);

	cout << "server started.\n";
	cout << "  master: " << povm["master"].as<string>() << "\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("server starting...");

	try
	{
		FleCSServer app;
		rc = app.main(argc, argv, "config.server");
	}
	catch (const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("server finished.");

	return rc;
}
